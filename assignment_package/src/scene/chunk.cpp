#include "chunk.h"


Chunk::Chunk(OpenGLContext* context, int x, int z) : Drawable(context), m_blocks(), minX(x), minZ(z), m_neighbors{{XPOS, nullptr}, {XNEG, nullptr}, {ZPOS, nullptr}, {ZNEG, nullptr}}
{
    std::fill_n(m_blocks.begin(), 65536, EMPTY);
}

// Does bounds checking with at()
BlockType Chunk::getLocalBlockAt(unsigned int x, unsigned int y, unsigned int z) const {
    return m_blocks.at(x + 16 * y + 16 * 256 * z);
}

// Exists to get rid of compiler warnings about int -> unsigned int implicit conversion
BlockType Chunk::getLocalBlockAt(int x, int y, int z) const {
    if (x >= 16 || x < 0) {
        return EMPTY;
    }

    if (y >= 256 || y < 0) {
        return EMPTY;
    }

    if (z >= 16 || z < 0) {
        return EMPTY;
    }
    return getLocalBlockAt(static_cast<unsigned int>(x), static_cast<unsigned int>(y), static_cast<unsigned int>(z));
}

// Does bounds checking with at()
void Chunk::setLocalBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t) {
    m_blocks.at(x + 16 * y + 16 * 256 * z) = t;
}


const static std::unordered_map<Direction, Direction, EnumHash> oppositeDirection {
    {XPOS, XNEG},
    {XNEG, XPOS},
    {YPOS, YNEG},
    {YNEG, YPOS},
    {ZPOS, ZNEG},
    {ZNEG, ZPOS}
};

void Chunk::linkNeighbor(uPtr<Chunk> &neighbor, Direction dir) {
    if(neighbor != nullptr) {
        this->m_neighbors[dir] = neighbor.get();
        neighbor->m_neighbors[oppositeDirection.at(dir)] = this;
    }
}

// struct VertexData {
//     glm::vec4 pos;
//     glm::vec4 nor;
//     glm::vec4 col;

//     VertexData(glm::vec4 position, glm::vec4 normal, glm::vec4 color)
//         : pos(position), nor(normal), col(color) {}
//     // VertexData makeOffSetVert(glm::vec3 pos, glm::vec3 col);
// };

glm::vec4 colorFromBlockType(BlockType t) {
    switch (t) {
    case GRASS:
        return glm::vec4(95.f, 159.f, 53.f,  255.f)/ 255.f;
    case DIRT:
        return glm::vec4(121.f, 85.f, 58.f,  255.f) / 255.f;
    case STONE:
        return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    case WATER:
        return glm::vec4(0.f, 0.f, 0.75f, 1.0f);
    case SAND:
        return glm::vec4(194.f, 178.f, 128.f,  255.f)/ 255.f;
    case SNOW:
        return glm::vec4(240.f, 236.f, 229.f,  255.f)/ 255.f;
    case LAVA:
        return glm::vec4(255.f, 0.f, 0.f,  255.f)/ 255.f;
    case BEDROCK:
        return glm::vec4(0.f, 0.f, 0.f,  1.f);
    default: // default debug purple
        return glm::vec4(1.f, 0.f, 1.f, 1.0f);
    }
}




boolean Chunk::neighborChunkBlockEmpty(int x, int y, int z, glm::ivec3 direction) {
    int directionX = direction.x;
    int directionY = direction.y;
    int directionZ = direction.z;

    if (directionX != 0) {
        if (directionX == 1) {
            Chunk* neighborChunk = m_neighbors[XPOS];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(0, y, z) != EMPTY) {
                    return false;
                }
            }
        }
        if (directionX == -1) {
            Chunk* neighborChunk = m_neighbors[XNEG];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(15, y, z) != EMPTY) {
                    return false;
                }
            }
        }
    }

    if (directionY != 0) {
        if (directionY == 1) {
            Chunk* neighborChunk = m_neighbors[YPOS];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(x, 0, z) != EMPTY) {
                    return false;
                }
            }
        }
        if (directionY == -1) {

            Chunk* neighborChunk = m_neighbors[YNEG];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(x, 255, z) != EMPTY) {
                    return false;
                }
            }
        }

    }

    if (directionZ != 0) {
        if (directionZ == 1) {
            Chunk* neighborChunk = m_neighbors[ZPOS];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(x, y, 0) != EMPTY) {
                    return false;
                }
            }
        }
        if (directionZ == -1) {
            Chunk* neighborChunk = m_neighbors[ZNEG];
            if (neighborChunk) {
                if (neighborChunk->getLocalBlockAt(x, y, 15) != EMPTY) {
                    return false;
                }
            }
        }
    }
    return true;

}

void Chunk::createVBOdata() {
    // set up pos, nor and col data in one VBO
    std::vector<VertexData> data;
    std::vector<GLuint> idx;
    unsigned int numFaces = 0;

    //iterate over each block in this chuck
    for (int z = 0; z < 16; ++z) {
        for (int y = 0; y < 256; ++y) {
            for (int x = 0; x < 16; ++x) {
                BlockType currentBlock = getLocalBlockAt(x, y, z);
                if (currentBlock != EMPTY) {
                    // check for surrounding faces, if neighbor is empty, then add that face for this current block
                    for (auto &neighbor : adjacentBlocks) {

                        int neighborX = x + neighbor.direction.x;
                        int neighborY = y + neighbor.direction.y;
                        int neighborZ = z + neighbor.direction.z;
                        if (neighborX >= 0 && neighborY >= 0 && neighborZ >= 0 && neighborX < 16 && neighborY < 256 && neighborZ < 16) {
                            if (getLocalBlockAt(neighborX, neighborY, neighborZ) == EMPTY) {

                                // add face
                                for (VertexData vert : neighbor.vertices) {
                                    vert.pos += glm::vec4(x + minX , y,  z + minZ, 0);
                                    // vert.col = colorFromBlockType(currentBlock);
                                    vert.uv = blockFaceUVs.at(currentBlock).at(neighbor.faceDir);

                                    data.push_back(vert);
                                }
                                // idx array
                                for (int i : {0, 1, 2, 0, 2, 3}) {
                                    idx.push_back(numFaces * 4 + i);
                                }
                                ++numFaces;

                            }

                        } else {
                            // check for neighboring chunk
                            if (neighborChunkBlockEmpty(x, y, z, neighbor.direction)) {
                                for (VertexData vert : neighbor.vertices) {
                                    vert.pos += glm::vec4(x + minX , y,  z + minZ, 0);
                                    // vert.col = colorFromBlockType(currentBlock);
                                    // vert.col = blockFaceUVs.at(currentBlock).at(XPOS);
                                    vert.uv = blockFaceUVs.at(currentBlock).at(neighbor.faceDir);

                                    data.push_back(vert);
                                }
                                // idx array
                                for (int i : {0, 1, 2, 0, 2, 3}) {
                                    idx.push_back(numFaces * 4 + i);
                                }
                                ++numFaces;
                            }
                        }
                    }
                }
            }
        }
    }

    indexCounts[INDEX] = idx.size();

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(GLuint), idx.data(), GL_STATIC_DRAW);

    generateBuffer(INTERLEAVED);
    bindBuffer(INTERLEAVED);
    mp_context->glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexData), data.data(), GL_STATIC_DRAW);
    readyToRender = true;

}

bool isTrans(BlockType t) {
    return t == WATER || t == LAVA;
}
// AdjacentFace, 6> adjacentBlocks
void Chunk::updateVBO(std::vector<glm::vec4> &interleave,
                      Direction dir, glm::vec4 pos,
                      BlockType blockType) {

    AdjacentFace neighbor = adjacentBlocks.at(dir);
    glm::vec4 neighbor_uv = blockFaceUVs.at(blockType).at(dir);

    for (int i = 0; i < 4; i++) {
        interleave.push_back(neighbor.vertices[i].pos + pos + glm::vec4(minX, 0, minZ, 0));
        interleave.push_back(glm::vec4(neighbor.direction, 1));

        if (blockType == WATER) {
            neighbor_uv.z = 1;
        }
        if (blockType == LAVA) {
            neighbor_uv.z = 2;
        }
        interleave.push_back(neighbor_uv + neighbor.vertices[i].uv);
    }
}

void Chunk::createVBOdataTexture(void) {
    std::vector<GLuint> idxOpq;
    std::vector<glm::vec4> dataOpq;

    std::vector<GLuint> idxTrans;
    std::vector<glm::vec4> dataTrans;

    int facesTrans = 0;
    int verticesTrans = 0;

    int facesOpq = 0;
    int verticesOpq = 0;

    // iterates over all 3 coords of chunks
    for (int x = 0; x < 16; ++x) {
        for (int y = 0; y < 256; ++y) {
            for (int z = 0; z < 16; ++z) {

                BlockType currBlock = getLocalBlockAt(x, y, z);
                glm::vec4 block(x, y, z, 0);

                if (currBlock != EMPTY) {

                    BlockType pos[6];// = getLocalBlockAt(x + 1, y, z);
                    pos[XPOS] = getLocalBlockAt(x + 1, y, z);
                    pos[XNEG] = getLocalBlockAt(x - 1, y, z);
                    pos[YPOS] = getLocalBlockAt(x, y + 1, z);
                    pos[YNEG] = getLocalBlockAt(x, y - 1, z);
                    pos[ZPOS] = getLocalBlockAt(x, y, z + 1);
                    pos[ZNEG] = getLocalBlockAt(x, y, z - 1);

                    if (x == 0 && m_neighbors[XNEG]) {
                        pos[XNEG] = m_neighbors[XNEG]->getLocalBlockAt(15, y, z);
                    }

                    if (x == 15 && m_neighbors[XPOS]) {
                        pos[XPOS] = m_neighbors[XPOS]->getLocalBlockAt((x + 1) % 16, y, z);
                    }

                    if (y == 0 || y == 255) {
                        pos[YNEG] = EMPTY;
                    }

                    if (z == 0 && m_neighbors[ZNEG]) {
                        pos[ZNEG] = m_neighbors[ZNEG]->getLocalBlockAt(x, y, 15);
                    }

                    if (z == 15 && m_neighbors[ZPOS]) {
                        pos[ZPOS] = m_neighbors[ZPOS]->getLocalBlockAt(x, y, 0);
                    }

                    for(Direction i : {XPOS, XNEG, YPOS, YNEG, ZPOS, ZNEG}) {
                        if (pos[i] == EMPTY || (isTrans(pos[i]) && pos[i] != currBlock)) {
                            if (!isTrans(currBlock)) {
                                updateVBO(dataOpq, i, block, currBlock);
                                facesOpq++;
                            } else {
                                updateVBO(dataTrans, i, block, currBlock);
                                facesTrans++;
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < facesOpq; i++) {
        idxOpq.push_back(verticesOpq);
        idxOpq.push_back(verticesOpq + 1);
        idxOpq.push_back(verticesOpq + 2);
        idxOpq.push_back(verticesOpq);
        idxOpq.push_back(verticesOpq + 2);
        idxOpq.push_back(verticesOpq + 3);
        verticesOpq += 4;
    }

    for (int i = 0; i < facesTrans; i++) {
        idxTrans.push_back(verticesTrans);
        idxTrans.push_back(verticesTrans + 1);
        idxTrans.push_back(verticesTrans + 2);
        idxTrans.push_back(verticesTrans);
        idxTrans.push_back(verticesTrans + 2);
        idxTrans.push_back(verticesTrans + 3);
        verticesTrans += 4;
    }

    indexCounts[IDX_OPQ] = idxOpq.size();

    generateBuffer(IDX_OPQ);
    bindBuffer(IDX_OPQ);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxOpq.size() * sizeof(GLuint), idxOpq.data(), GL_STATIC_DRAW);

    generateBuffer(DATA_OPQ);
    bindBuffer(DATA_OPQ);
    mp_context->glBufferData(GL_ARRAY_BUFFER, dataOpq.size() * sizeof(glm::vec4), dataOpq.data(), GL_STATIC_DRAW);

    indexCounts[IDX_TRANS] = idxTrans.size();

    generateBuffer(IDX_TRANS);
    bindBuffer(IDX_TRANS);
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxTrans.size() * sizeof(GLuint), idxTrans.data(), GL_STATIC_DRAW);

    generateBuffer(DATA_TRANS);
    bindBuffer(DATA_TRANS);
    mp_context->glBufferData(GL_ARRAY_BUFFER, dataTrans.size() * sizeof(glm::vec4), dataTrans.data(), GL_STATIC_DRAW);

    readyToRender = true;

}

void Chunk::buildVBODataForChunk(Chunk *chunk, chunkVBOData data) {
    // chunk->createVBOdata();
    chunk->createVBOdataTexture();
    // should call create for texture VBO
}

void Chunk::create(const std::vector<float> &vboDataOpaque, const std::vector<GLuint> &idxDataOpaque, const std::vector<float> &vboDataTransparent, const std::vector<GLuint> &idxDataTransparent) {
}
