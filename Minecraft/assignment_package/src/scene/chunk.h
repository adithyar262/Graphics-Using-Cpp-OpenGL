#pragma once
#include "smartpointerhelp.h"
#include "glm_includes.h"
#include <array>
#include <unordered_map>
#include <cstddef>
#include <drawable.h>


#define MAX_X 16
#define MAX_Y 256
#define MAX_Z 16
//using namespace std;

// C++ 11 allows us to define the size of an enum. This lets us use only one byte
// of memory to store our different block types. By default, the size of a C++ enum
// is that of an int (so, usually four bytes). This *does* limit us to only 256 different
// block types, but in the scope of this project we'll never get anywhere near that many.
enum BlockType : unsigned char
{
    EMPTY, GRASS, DIRT, STONE, WATER, SNOW, SAND, LAVA, BEDROCK, SHRUB, WOOD, LEAF
};

// The six cardinal directions in 3D space
enum Direction : unsigned char
{
    XPOS, XNEG, YPOS, YNEG, ZPOS, ZNEG
};

// Lets us use any enum class as the key of a
// std::unordered_map
struct EnumHash {
    template <typename T>
    size_t operator()(T t) const {
        return static_cast<size_t>(t);
    }
};

const static std::unordered_map<BlockType, std::unordered_map<Direction, glm::vec4, EnumHash>, EnumHash> blockFaceUVs {
    {GRASS, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(3.f/16.f, 15.f/16.f, 0, 0)},
                                                               {XNEG, glm::vec4(3.f/16.f, 15.f/16.f, 0, 0)},
                                                               {YPOS, glm::vec4(8.f/16.f, 13.f/16.f, 0, 0)},
                                                               {YNEG, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                               {ZPOS, glm::vec4(3.f/16.f, 15.f/16.f, 0, 0)},
                                                               {ZNEG, glm::vec4(3.f/16.f, 15.f/16.f, 0, 0)}}},

    {DIRT, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(2.f/16.f, 15.f/16.f, 0, 0)}}},

    {STONE, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(1.f/16.f, 15.f/16.f, 0, 0)},
                                                               {XNEG, glm::vec4(0.f/16.f, 15.f/16.f, 0, 0)},
                                                               {YPOS, glm::vec4(1.f/16.f, 15.f/16.f, 0, 0)},
                                                               {YNEG, glm::vec4(1.f/16.f, 15.f/16.f, 0, 0)},
                                                               {ZPOS, glm::vec4(0.f/16.f, 15.f/16.f, 0, 0)},
                                                               {ZNEG, glm::vec4(1.f/16.f, 15.f/16.f, 0, 0)}}},

    {LAVA, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(15.f/16.f, 1.f/16.f, 0, 0)}}},

    {WATER, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)},
                                                               {XNEG, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)},
                                                               {YPOS, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)},
                                                               {YNEG, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)},
                                                               {ZPOS, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)},
                                                               {ZNEG, glm::vec4(15.f/16.f, 3.f/16.f, 0, 0)}}},

    {SNOW, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(2.f/16.f, 11.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(5.f/16.f, 8.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(2.f/16.f, 11.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(2.f/16.f, 11.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(5.f/16.f, 8.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(5.f/16.f, 8.f/16.f, 0, 0)}}},

    {SAND, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(2.f/16.f, 14.f/16.f, 0, 0)}}},

    {BEDROCK, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)},
                                                                 {XNEG, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)},
                                                                 {YPOS, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)},
                                                                 {YNEG, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)},
                                                                 {ZPOS, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)},
                                                                 {ZNEG, glm::vec4(5.f/16.f, 3.f/16.f, 0, 0)}}},

    {SHRUB, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)},
                                                                 {XNEG, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)},
                                                                 {YPOS, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)},
                                                                 {YNEG, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)},
                                                                 {ZPOS, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)},
                                                                 {ZNEG, glm::vec4(15.f/16.f, 15.f/16.f, 0, 0)}}},

    {WOOD, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(6.f/16.f, 10.f/16.f, 0, 0)}}},

    {LEAF, std::unordered_map<Direction, glm::vec4, EnumHash>{{XPOS, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)},
                                                              {XNEG, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)},
                                                              {YPOS, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)},
                                                              {YNEG, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)},
                                                              {ZPOS, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)},
                                                              {ZNEG, glm::vec4(4.f/16.f, 12.f/16.f, 0, 0)}}}

};

typedef enum : char {
    RENDER_TODO = 0,
    RENDER_OPQ,
    RENDER_TRANS,
    RENDER_DONE
}RenderState_t;

struct VertexData {
    glm::vec4 pos;
    glm::vec4 uv;

    VertexData(glm::vec4 position, glm::vec4 color)
        : pos(position), uv(color) {}
    // VertexData makeOffSetVert(glm::vec3 pos, glm::vec3 col);
};

struct AdjacentFace {
    glm::ivec3 direction;
    Direction faceDir;
    std::array<VertexData, 4> vertices;

    AdjacentFace(Direction face, glm::ivec3 dir, std::array<VertexData, 4> ver)
        : direction(dir), faceDir(face), vertices(ver) {}
};


const static std::array<AdjacentFace, 6> adjacentBlocks = {
    AdjacentFace(
        XPOS,
        glm::ivec3(1, 0, 0),
        {
            VertexData(glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(1, 0, 0, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(1, 1, 0, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(1, 1, 1, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        ),
    AdjacentFace(
        XNEG,
        glm::ivec3(-1, 0, 0),
        {
            VertexData(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(0, 0, 1, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(0, 1, 1, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(0, 1, 0, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        ),
    AdjacentFace(
        YPOS,
        glm::ivec3(0, 1, 0),
        {
            VertexData(glm::vec4(0, 1, 1, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(1, 1, 1, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(1, 1, 0, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(0, 1, 0, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        ),
    AdjacentFace(
        YNEG,
        glm::ivec3(0, -1, 0),
        {
            VertexData(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(1, 0, 0, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(1, 0, 1, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(0, 0, 1, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        ),
    AdjacentFace(
        ZPOS,
        glm::ivec3(0, 0, 1),
        {
            VertexData(glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(1, 0, 1, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(1, 1, 1, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(0, 1, 1, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        ),
    AdjacentFace(
        ZNEG,
        glm::ivec3(0, 0, -1),
        {
            VertexData(glm::vec4(1, 0, 0, 1), glm::vec4(0, 0, 0, 0)),
            VertexData(glm::vec4(0, 0, 0, 1), glm::vec4(0.0625, 0, 0, 0)),
            VertexData(glm::vec4(0, 1, 0, 1), glm::vec4(0.0625, 0.0625, 0, 0)),
            VertexData(glm::vec4(1, 1, 0, 1), glm::vec4(0, 0.0625, 0, 0))
        }
        )

};

// One Chunk is a 16 x 256 x 16 section of the world,
// containing all the Minecraft blocks in that area.
// We divide the world into Chunks in order to make
// recomputing its VBO data faster by not having to
// render all the world at once, while also not having
// to render the world block by block.

// TODO have Chunk inherit from Drawable

struct chunkVBOData;

class Chunk : public Drawable {
private:
    // All of the blocks contained within this Chunk
    std::array<BlockType, 65536> m_blocks;
    // The coordinates of the chunk's lower-left corner in world space
    int minX, minZ;
    // This Chunk's four neighbors to the north, south, east, and west
    // The third input to this map just lets us use a Direction as
    // a key for this map.
    // These allow us to properly determine
    std::unordered_map<Direction, Chunk*, EnumHash> m_neighbors;
    boolean hasRendered = false;
    boolean readyToRender = false;

    boolean neighborChunkBlockEmpty(int x, int y, int z, glm::ivec3 direction);

public:
    Chunk(OpenGLContext* context, int x, int z);
    BlockType getLocalBlockAt(unsigned int x, unsigned int y, unsigned int z) const;
    BlockType getLocalBlockAt(int x, int y, int z) const;
    void setLocalBlockAt(unsigned int x, unsigned int y, unsigned int z, BlockType t);
    void linkNeighbor(uPtr<Chunk>& neighbor, Direction dir);
    void createVBOdata() override;
    void createVBOdataTexture(void);
    void updateVBO(std::vector<glm::vec4> &interleave,
                   Direction dir, glm::vec4 pos,
                   BlockType blockType);
    static void buildVBODataForChunk(Chunk *chunk, chunkVBOData data);
    void create(const std::vector<float> &vboDataOpaque, const std::vector<GLuint> &idxDataOpaque, const std::vector<float> &vboDataTransparent, const std::vector<GLuint> &idxDataTransparent);

    friend class Terrain;
    friend class FBMWorker;
    friend class VBOWorker;
};

struct chunkVBOData {
    Chunk* mp_chunk;
    std::vector<float> m_vboDataOpaque,m_vboDataTransparent;
    std::vector<GLuint> m_idxDataOpaque, m_idxDataTransparent;

    chunkVBOData(Chunk* c): mp_chunk(c), m_vboDataOpaque{}, m_vboDataTransparent{},
        m_idxDataOpaque{}, m_idxDataTransparent{}
    {}
};

