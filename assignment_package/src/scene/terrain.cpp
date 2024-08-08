#include "terrain.h"
#include "cube.h"
#include "biomes.h"
#include <stdexcept>
#include <iostream>
#include <QThreadPool>
#include "fbmworker.cpp"
#include "vboworker.cpp"
#include <stack>

#include "../lsystem.cpp"

Terrain::Terrain(OpenGLContext *context)
    : m_chunks(), m_generatedTerrain(), m_geomCube(context),
    m_chunkVBOsNeedUpdating(true), mp_context(context), terrainGenerationOrientation(0,0,0)
{
    std::unordered_map<char, std::string> rules = {
        {'I', "FFR[X]"},
        {'X', "F-R+[-I+]"},
        {'R', "+FI-[L+S-]"},
        {'S', "F+I"},
        {'L', "F-I"}
    };

    LSystem lsystem("I", rules, 6);
    this->lsystemString = lsystem.generate();
}
Terrain::~Terrain() {
    m_geomCube.destroyVBOdata();
}

// Combine two 32-bit ints into one 64-bit int
// where the upper 32 bits are X and the lower 32 bits are Z
int64_t toKey(int x, int z) {
    int64_t xz = 0xffffffffffffffff;
    int64_t x64 = x;
    int64_t z64 = z;

    // Set all lower 32 bits to 1 so we can & with Z later
    xz = (xz & (x64 << 32)) | 0x00000000ffffffff;

    // Set all upper 32 bits to 1 so we can & with XZ
    z64 = z64 | 0xffffffff00000000;

    // Combine
    xz = xz & z64;
    return xz;
}

glm::ivec2 toCoords(int64_t k) {
    // Z is lower 32 bits
    int64_t z = k & 0x00000000ffffffff;
    // If the most significant bit of Z is 1, then it's a negative number
    // so we have to set all the upper 32 bits to 1.
    // Note the 8    V
    if(z & 0x0000000080000000) {
        z = z | 0xffffffff00000000;
    }
    int64_t x = (k >> 32);

    return glm::ivec2(x, z);
}

// Surround calls to this with try-catch if you don't know whether
// the coordinates at x, y, z have a corresponding Chunk
BlockType Terrain::getGlobalBlockAt(int x, int y, int z) const
{
    if(hasChunkAt(x, z)) {
        // Just disallow action below or above min/max height,
        // but don't crash the game over it.
        if(y < 0 || y >= 256) {
            return EMPTY;
        }
        const uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        return c->getLocalBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                                  static_cast<unsigned int>(y),
                                  static_cast<unsigned int>(z - chunkOrigin.y));
    }
    else {
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

BlockType Terrain::getGlobalBlockAt(glm::vec3 p) const {
    return getGlobalBlockAt(p.x, p.y, p.z);
}

bool Terrain::hasChunkAt(int x, int z) const {
    // Map x and z to their nearest Chunk corner
    // By flooring x and z, then multiplying by 16,
    // we clamp (x, z) to its nearest Chunk-space corner,
    // then scale back to a world-space location.
    // Note that floor() lets us handle negative numbers
    // correctly, as floor(-1 / 16.f) gives us -1, as
    // opposed to (int)(-1 / 16.f) giving us 0 (incorrect!).
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    return m_chunks.find(toKey(16 * xFloor, 16 * zFloor)) != m_chunks.end();
}


uPtr<Chunk>& Terrain::getChunkAt(int x, int z) {
    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));
    if (m_chunks.find(toKey(16 * xFloor, 16 * zFloor)) == m_chunks.end()) {
        std::cout << "Chunk " << x << " , " << z << " is not found." << std::endl;
    }
    return m_chunks[toKey(16 * xFloor, 16 * zFloor)];
}


const uPtr<Chunk>& Terrain::getChunkAt(int x, int z) const {

    int xFloor = static_cast<int>(glm::floor(x / 16.f));
    int zFloor = static_cast<int>(glm::floor(z / 16.f));

    return m_chunks.at(toKey(16 * xFloor, 16 * zFloor));
}

void Terrain::setGlobalBlockAt(int x, int y, int z, BlockType t)
{
    if(hasChunkAt(x, z)) {
        uPtr<Chunk> &c = getChunkAt(x, z);
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        c->setLocalBlockAt(static_cast<unsigned int>(x - chunkOrigin.x),
                           static_cast<unsigned int>(y),
                           static_cast<unsigned int>(z - chunkOrigin.y),
                           t);
        c->hasRendered = false;
    }
    else {
        glm::vec2 chunkOrigin = glm::vec2(floor(x / 16.f) * 16, floor(z / 16.f) * 16);
        std::cout << "Can't find chunk origin at: " << chunkOrigin.x << " , " << chunkOrigin.y << std::endl;
        throw std::out_of_range("Coordinates " + std::to_string(x) +
                                " " + std::to_string(y) + " " +
                                std::to_string(z) + " have no Chunk!");
    }
}

Chunk* Terrain::instantiateChunkAt(int x, int z) {
    uPtr<Chunk> chunk = mkU<Chunk>(mp_context, x, z);
    Chunk *cPtr = chunk.get();
    m_chunks[toKey(x, z)] = std::move(chunk);
    // Set the neighbor pointers of itself and its neighbors
    if(hasChunkAt(x, z + 16)) {
        auto &chunkNorth = m_chunks[toKey(x, z + 16)];
        cPtr->linkNeighbor(chunkNorth, ZPOS);
    }
    if(hasChunkAt(x, z - 16)) {
        auto &chunkSouth = m_chunks[toKey(x, z - 16)];
        cPtr->linkNeighbor(chunkSouth, ZNEG);
    }
    if(hasChunkAt(x + 16, z)) {
        auto &chunkEast = m_chunks[toKey(x + 16, z)];
        cPtr->linkNeighbor(chunkEast, XPOS);
    }
    if(hasChunkAt(x - 16, z)) {
        auto &chunkWest = m_chunks[toKey(x - 16, z)];
        cPtr->linkNeighbor(chunkWest, XNEG);
    }
    return cPtr;
}

void Terrain::drawInitial(ShaderProgram *shaderProgram) {
    for (auto zoneKey: m_generatedTerrain) {
        glm::ivec2 zoneCord = toCoords(zoneKey);
        for (int x = 0; x < 4; ++x) {
            for (int z = 0; z < 4; ++z) {
                int chunkX = zoneCord.x + x * 16;
                int chunkZ = zoneCord.y + z * 16;
                if (hasChunkAt(chunkX, chunkZ)) {
                    const uPtr<Chunk> &chunk = getChunkAt(chunkX, chunkZ);
                    if (!chunk->hasRendered) {
                        chunkVBOData vboData(chunk.get());
                        chunk->buildVBODataForChunk(chunk.get(), vboData);
                        chunk->hasRendered = true;
                    }
                    if (chunk->readyToRender) {
                        shaderProgram->drawInterleaved(*chunk);
                    }
                }
            }
        }
    }

}


void Terrain::draw(ShaderProgram *shaderProgram) {

    for (auto zoneKey: m_generatedTerrain) {

        glm::ivec2 zoneCord = toCoords(zoneKey);

        for (int x = 0; x < 4; ++x) {
            for (int z = 0; z < 4; ++z) {
                int chunkX = zoneCord.x + x * 16;
                int chunkZ = zoneCord.y + z * 16;
                if (hasChunkAt(chunkX, chunkZ)) {
                    const uPtr<Chunk> &chunk = getChunkAt(chunkX, chunkZ);
                    if (!chunk->hasRendered) {
                        chunkVBOData vboData(chunk.get());
                        chunk->buildVBODataForChunk(chunk.get(), vboData);
                        chunk->hasRendered = true;
                    }
                    if (chunk->readyToRender) {
                        // shaderProgram->drawInterleaved(*chunk);
                        shaderProgram->drawOpaque(*chunk);
                        shaderProgram->drawTransparent(*chunk);
                    }
                }
            }
        }
    }


}

BlockType Terrain::getBlockType(int height, bool isTop) {

    if (height < 128) {
        return STONE;
    }

    if (height < 160) {
        return isTop ? GRASS : DIRT;
    }

    if (height > 200) {
        return isTop ? SNOW : STONE;
    }

    return STONE;
}

void Terrain::CreateTestScene()
{

    QSet<int64_t> initialZones;

    // create 5*5 for test scene
    for(int x = 0; x < 320; x += 64) {
        for(int z = 0; z < 320; z += 64) {
            glm::ivec2 zoneCoord(64.f * glm::floor(x / 64.f), 64.f * glm::floor(z / 64.f));
            int64_t zoneKey = toKey(zoneCoord.x, zoneCoord.y);
            initialZones.insert(zoneKey);
        }
    }
    spawnFBMWorkers(initialZones);
}


void Terrain::updateTerrainBasedOnPlayerPosition(const glm::vec3 playerPos) {


    uPtr<Chunk> &curChunkX = getChunkAt(playerPos.x, playerPos.z);
    int curMinX = curChunkX->minX;

    int curMinZ = curChunkX->minZ;

    if(!hasChunkAt(curMinX, curMinZ + 16)) {
        instantiateChunkAt(curMinX, curMinZ + 16);
    }
    if(!hasChunkAt(curMinX, curMinZ - 16)) {
        instantiateChunkAt(curMinX, curMinZ - 16);
    }
    if(!hasChunkAt(curMinX + 16, curMinZ)) {
        instantiateChunkAt(curMinX + 16, curMinZ);
    }
    if(!hasChunkAt(curMinX - 16, curMinZ)) {
        instantiateChunkAt(curMinX - 16, curMinZ);
    }

}

void Terrain::multiTheadedWorker(glm::vec3 playerPos, glm::vec3 playerPrevPos, float dT) {
    expansionWaitTime += dT;
    tryExpansion(playerPos, playerPrevPos);
    // checkThreadResult();
    expansionWaitTime = 0.f;
}


void Terrain::tryExpansion(glm::vec3 playerPos, glm::vec3 playerPrevPos) {
    // gets current and previous zone
    glm::ivec2 curZone(64.f * glm::floor(playerPos.x / 64.f), 64.f * glm::floor(playerPos.z / 64.f));
    glm::ivec2 preZone(64.f * glm::floor(playerPrevPos.x / 64.f), 64.f * glm::floor(playerPrevPos.z / 64.f));

    if (curZone.x != preZone.x || curZone.y != preZone.y) {
        QSet<int64_t> prevZoneBorders = getZoneBorders(preZone);
        QSet<int64_t> curZoneBorders = getZoneBorders(curZone);

        QSet<int64_t> zonesToGenerate = curZoneBorders - prevZoneBorders;

        spawnFBMWorkers(zonesToGenerate);
    }
}

QSet<int64_t> Terrain::getZoneBorders(glm::ivec2 zoneCoord) {
    QSet<int64_t> borderZones;
    // 4*16
    int zoneSize = 64;

    for (int dx = -2; dx <= 2; ++dx) {
        for (int dz = -2; dz <= 2; ++dz) {
            int neighborX = zoneCoord.x + dx * zoneSize;
            int neighborZ = zoneCoord.y + dz * zoneSize;
            int64_t zoneKey = toKey(neighborX, neighborZ);
            borderZones.insert(zoneKey);
        }
    }

    return borderZones;
}

void Terrain::spawnFBMWoker(int64_t zoneKey) {
    FBMWorker *worker = new FBMWorker(zoneKey, this, &m_chunksThatHaveBlockDataLock);
    QThreadPool::globalInstance()->start(worker);

}

void Terrain::spawnFBMWorkers(const QSet<int64_t> &zonesToGenerate) {
    for (int64_t zone: zonesToGenerate) {
        if (m_generatedTerrain.find(zone) == m_generatedTerrain.end()) {
            m_generatedTerrain.insert(zone);
            spawnFBMWoker(zone);
        }
    }
}

void Terrain::spawnVBOWorker(Chunk* chunkNeedingVBOData) {
    VBOWorker *worker = new VBOWorker(chunkNeedingVBOData, &m_chunksThatHaveVBOsLock, this);
    QThreadPool::globalInstance()->start(worker);

}

void Terrain::spawnVBOWorkers(const std::unordered_set<Chunk*> &chunksNeedingVBOs) {
    for (Chunk* c : chunksNeedingVBOs) {
        spawnVBOWorker(c);
    }

}

void Terrain::checkThreadResult() {
    m_chunksThatHaveBlockDataLock.lock();
    spawnVBOWorkers(m_chunksThatHaveBlockData);
    m_chunksThatHaveBlockData.clear();
    m_chunksThatHaveBlockDataLock.unlock();
    m_chunksThatHaveVBOsLock.lock();
    // build texture data
    for (chunkVBOData &cd : m_chunksThatHaveVBOs) {
        cd.mp_chunk->create(cd.m_vboDataOpaque, cd.m_idxDataOpaque, cd.m_vboDataTransparent, cd.m_idxDataTransparent);
    }
    m_chunksThatHaveVBOs.clear();
    m_chunksThatHaveVBOsLock.unlock();
}


glm::vec2 rotate(const glm::vec2& v, double angleDeg) {
    double rad = angleDeg * M_PI / 180.0;
    return glm::vec2(
        v.x * cos(rad) - v.y * sin(rad),
        v.x * sin(rad) + v.y * cos(rad)
        );
}

void checkLinePassingChunk(glm::ivec2 start, glm::ivec2 end, int chunkX, int chunkZ, std::vector<glm::ivec2> &river) {
    // Adjusted chunk boundaries
    glm::ivec2 chunkMin(chunkX - 8, chunkZ - 8);
    glm::ivec2 chunkMax(chunkX + 24, chunkZ + 24);

    // Implement a line drawing algorithm (e.g., Bresenham's)
    // that iterates through all the voxels from start to end
    // and checks if each voxel is within the adjusted chunk boundaries.

    int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2; // error value e_xy

    while (true) {
        // Check if the current point is within the chunk boundaries
        if (start.x >= chunkMin.x && start.x <= chunkMax.x &&
            start.y >= chunkMin.y && start.y <= chunkMax.y) {
            river.push_back(start); // Add point to vector
        }

        if (start == end) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
    }
}


void Terrain::generateRiver(int chunkX, int chunkZ, std::vector<glm::ivec2> &river){
    glm::vec2 pos(268.0f, -208.0f);
    double angle = 79.0f;

    std::stack<glm::vec2> positionStack;
    std::stack<double> angleStack;

    const double lineLength = 24.0;
    const double angleIncrement = -31.0;



    for (char c : this->lsystemString) {
        switch (c) {
        case 'F': {
            glm::vec2 direction = rotate(glm::vec2(1.0f, 0.0f), angle);
            glm::vec2 newPos = pos + direction * static_cast<float>(lineLength);

            glm::vec2 curvPos1 = pos + rotate(direction, 5.0f) * static_cast<float>(lineLength * 0.4);
            glm::vec2 curvPos2 = pos + rotate(direction, -5.0f) * static_cast<float>(lineLength * 0.7);

            // Check if the line crosses the chunk boundary
            checkLinePassingChunk(glm::ivec2(static_cast<int>(std::round(pos.x)), static_cast<int>(std::round(pos.y))),
                                  glm::ivec2(static_cast<int>(std::round(curvPos1.x)), static_cast<int>(std::round(curvPos1.y))),
                                  chunkX, chunkZ, river);
            checkLinePassingChunk(glm::ivec2(static_cast<int>(std::round(curvPos1.x)), static_cast<int>(std::round(curvPos1.y))),
                                  glm::ivec2(static_cast<int>(std::round(curvPos2.x)), static_cast<int>(std::round(curvPos2.y))),
                                  chunkX, chunkZ, river);
            checkLinePassingChunk(glm::ivec2(static_cast<int>(std::round(curvPos2.x)), static_cast<int>(std::round(curvPos2.y))),
                                  glm::ivec2(static_cast<int>(std::round(newPos.x)), static_cast<int>(std::round(newPos.y))),
                                  chunkX, chunkZ, river);

            pos = newPos;
            break;
        }
        case '+':
            angle -= angleIncrement;
            break;
        case '-':
            angle += angleIncrement;
            break;
        case '[':
            positionStack.push(pos);
            angleStack.push(angle);
            break;
        case ']':
            if (!positionStack.empty() && !angleStack.empty()) {
                pos = positionStack.top();
                angle = angleStack.top();
                positionStack.pop();
                angleStack.pop();
            }
            break;
        default:
            break;
        }
    }
}


