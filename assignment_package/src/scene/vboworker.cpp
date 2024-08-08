#include <QRunnable>
#include <QMutex>
#include <QMutexLocker>
#include "chunk.h"
#include <iostream>
#include "terrain.h"

class VBOWorker : public QRunnable {
public:
    VBOWorker(Chunk* chunk, QMutex* mutex, Terrain* terrian)
        : m_chunk(chunk), m_mutex(mutex),  m_terrian(terrian) {
        setAutoDelete(true);
    }

    void run() override {
        if (!m_chunk->hasRendered) {          
            chunkVBOData vboData(m_chunk);
            m_chunk->buildVBODataForChunk(m_chunk, vboData);
            m_terrian->m_chunksThatHaveVBOs.push_back(vboData);
            m_chunk->hasRendered = true;
        }      
    }

private:
    Chunk* m_chunk;
    QMutex* m_mutex;
    Terrain* m_terrian;
};
