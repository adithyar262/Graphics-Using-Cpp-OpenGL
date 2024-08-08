#include <QRunnable>
#include <QMutex>
#include <QMutexLocker>
#include <iostream>
#include <unordered_set>
#include "chunk.h"
#include "terrain.h"
#include "biomes.h"

class FBMWorker : public QRunnable {

private:

    int64_t m_zoneKey;
    Terrain* m_terrian;
    QMutex* m_mutex;

public:
    FBMWorker(int64_t zoneKey, Terrain* terrain, QMutex* mutex)
        : m_zoneKey(zoneKey), m_terrian(terrain), m_mutex(mutex) {
        setAutoDelete(true);
    }

    void run() override {
        generateChunks(m_zoneKey);
    }

    void generateChunks(int64_t zoneKey) {
        glm::ivec2 zoneCord = toCoords(zoneKey);

        for (int x = 0; x < 4; ++x) {
            for (int z = 0; z < 4; ++z) {
                int chunkX = zoneCord.x + x * 16;
                int chunkZ = zoneCord.y + z * 16;
                m_mutex->lock();

                Chunk* newChunk = m_terrian->instantiateChunkAt(chunkX, chunkZ);
                for (int bx = 0; bx < 16; ++bx) {
                    for (int bz = 0; bz < 16; ++bz) {

                        int globalX = chunkX + bx;
                        int globalZ = chunkZ + bz;

                        int height = Biomes::getHeight(globalX, globalZ);
                        if (height <= 0) {
                            m_terrian->setGlobalBlockAt(globalX, 0, globalZ, BEDROCK);
                            continue;
                        }
                        m_terrian->setGlobalBlockAt(globalX, 0, globalZ, BEDROCK);
                        // int range = height > 128 ? (height == 148 ? 148 : 128): height - 4;
                        int range = height  == 148 ? 148 : height - 8;
                        for (int k = 1; k <= height; k++) {
                            //setGlobalBlockAt(globalX, k, globalZ, getBlockType(height, k == height));
                            if (k <= range) {
                                float noise = Biomes::perlinNoise3D(glm::vec3(globalX, k, globalZ) / 100.0f, 1);
                                if (noise < - 0.1) {
                                    if (k < 25) {
                                        m_terrian->setGlobalBlockAt(globalX, k, globalZ, LAVA);
                                        // m_terrian->setGlobalBlockAt(globalX, k, globalZ, m_terrian->getBlockType(height, k == height));
                                    } else {
                                        m_terrian->setGlobalBlockAt(globalX, k, globalZ, EMPTY);
                                    }
                                } else {
                                    m_terrian->setGlobalBlockAt(globalX, k, globalZ, m_terrian->getBlockType(height, k == height));
                                }
                            } else {
                                m_terrian->setGlobalBlockAt(globalX, k, globalZ, m_terrian->getBlockType(height, k == height));
                            }
                            if(k == height && k < 250 && bx < 14 && bz < 14 && bx > 1 && bz > 1) {
                                if(rand() % 500 == 5) {
                                    if(m_terrian->getGlobalBlockAt(globalX, k, globalZ) == GRASS) { //Tree
                                        m_terrian->setGlobalBlockAt(globalX, k + 1, globalZ, WOOD);
                                        m_terrian->setGlobalBlockAt(globalX, k + 2, globalZ, WOOD);
                                        for (int y = k + 3; y < k + 7; y++) {
                                            if(y == k + 3) {
                                                for (int i = globalX - 1; i <= globalX + 1; i++) {
                                                    for (int j = globalZ - 1; j <= globalZ + 1; j++) {
                                                        m_terrian->setGlobalBlockAt(i, y, j, LEAF);
                                                    }
                                                }
                                            } else {

                                                for (int i = globalX - 1; i <= globalX + 1; i++) {
                                                    for (int j = globalZ - 1; j <= globalZ + 1; j++) {
                                                        if(rand() % 10 < 8)
                                                            m_terrian->setGlobalBlockAt(i, y, j, LEAF);
                                                    }
                                                }
                                            }
                                        }
                                    }

                                }
                            }
                        }

                        if (height < 128 && height > 0) {
                            for (int w = height + 1; w <= 128; ++w) {
                                m_terrian->setGlobalBlockAt(globalX, w, globalZ, WATER);
                            }
                        }
                        for (int k = 128; k <= 138; k++) {
                            if(m_terrian->getGlobalBlockAt(globalX, k, globalZ) == EMPTY &&
                                m_terrian->getGlobalBlockAt(globalX, k + 1, globalZ) == EMPTY && height > 138) {
                                m_terrian->setGlobalBlockAt(globalX, k, globalZ, WATER);
                            }
                        }
                    }
                }

                std::vector<glm::ivec2> river;
                m_terrian->generateRiver(chunkX, chunkZ, river);
                if (!river.empty()) {
                    int arr[16][16];
                    memset(arr, -1, sizeof(arr));
                    for (glm::ivec2 vec : river) {
                        int height = Biomes::getHeight(vec.x, vec.y);
                        int radius = -1;
                        if (height <= 132) {
                            radius = 6;
                        } else if (height > 132 && height <= 144) {
                            radius = 5;
                        } else if (height > 144 && height <= 156) {
                            radius = 3;
                        } else if (height > 156 && height <= 178) {
                            radius = 1;
                        } else if (height > 178) {
                            radius = 0;
                        }
                        if (radius == -1) continue;

                        int depth = radius + 1;
                        for (int offsetX = -radius; offsetX <= radius; ++offsetX) {
                            for (int offsetZ = -radius; offsetZ <= radius; ++offsetZ) {
                                int coorX  = vec.x + offsetX;
                                int coorZ  = vec.y + offsetZ;
                                if ( coorX >= chunkX && coorX < chunkX + 16 && coorZ >= chunkZ && coorZ < chunkZ + 16) {
                                    int purposedDepth = depth - (abs(offsetX) >= abs(offsetZ) ? abs(offsetX) : abs(offsetZ));
                                    arr[coorX - chunkX][coorZ - chunkZ] = arr[coorX - chunkX][coorZ - chunkZ] > purposedDepth ? arr[coorX - chunkX][coorZ - chunkZ] : purposedDepth;
                                }
                            }
                        }
                    }

                    for (int stepX = 0; stepX < 16; ++stepX) {
                        for (int stepZ = 0; stepZ < 16; ++stepZ) {
                            if (arr[stepX][stepZ] == -1) continue;
                            int height = Biomes::getHeight(chunkX + stepX, chunkZ + stepZ);
                            for (int stepY = 0; stepY <= arr[stepX][stepZ] + 1; ++stepY) {
                                m_terrian->setGlobalBlockAt(chunkX + stepX, height - stepY, chunkZ + stepZ, stepY > 1 ? WATER : height - stepY > 128 ? EMPTY : WATER);
                            }
                        }
                    }
                }


                //m_terrian->m_chunksThatHaveBlockData.insert(newChunk);
                m_mutex->unlock();
            }
        }

    }

};
