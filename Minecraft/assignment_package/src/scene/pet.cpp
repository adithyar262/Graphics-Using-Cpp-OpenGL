#include "pet.h"

#define PET_IDX_COUNT 6
#define PET_VERT_COUNT 4

Pet::Pet(OpenGLContext *context) : Drawable(context)
{}

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createBirdVertexPositionsBody(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT], glm::vec4 playerPos)
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 1.0f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 1.0f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 1.0f, 1.0f) + playerPos;

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 0.6f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 1.0f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + playerPos;

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 1.0f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 1.0f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 0.6f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f) + playerPos;

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 0.6f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 0.6f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f) + playerPos;

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 1.0f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f) + playerPos;

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 1.0f, 1.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(1.0f, 0.85f, 0.6f, 1.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 0.6f, 1.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.6f, 0.85f, 1.0f, 1.0f) + playerPos;
}


void createBirdVertexNormalsBody(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,1,0);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(1,0,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(-1,0,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,-1,0);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,1,0,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,-1,0,0);
    }
}

void createBirdIndicesBody(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Pet::updateVBOdataBody(glm::vec3 p)
{
    glm::vec4 playerPos(p.x + 1, p.y + 0.7, p.z - 1, 0.f);
    GLuint sph_idx[CUB_IDX_COUNT];
    glm::vec4 sph_vert_pos[CUB_VERT_COUNT];
    glm::vec4 sph_vert_nor[CUB_VERT_COUNT];
    glm::vec4 sph_vert_col[CUB_VERT_COUNT];

    for(int i = 0; i < CUB_VERT_COUNT; ++i) {
        sph_vert_col[i] = glm::vec4(7.f/16.f, 1.f/16.f, 0, 0);
    }

    createBirdVertexPositionsBody(sph_vert_pos, playerPos);
    createBirdVertexNormalsBody(sph_vert_nor);
    createBirdIndicesBody(sph_idx);

    indexCounts[INDEX] = CUB_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateBuffer(INDEX);
    bindBuffer(INDEX);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(GLuint), sph_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_pos, GL_STATIC_DRAW);

    generateBuffer(NORMAL);
    bindBuffer(NORMAL);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_nor, GL_STATIC_DRAW);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_col, GL_STATIC_DRAW);

}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createBirdVertexPositionsWings(glm::vec4 (&cub_vert_pos)[CUB_VERT_COUNT], glm::vec4 playerPos, int side)
{
    int idx = 0;

    float xEnd = (side == 0) ? 0.2 : 1.4;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.9f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.9f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.9f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.9f, 84.0f) + playerPos;

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.7f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.7f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.9f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.9f, 84.0f) + playerPos;

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.9f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.9f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.7f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.7f, 84.0f) + playerPos;

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.7f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.7f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.7f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.7f, 84.0f) + playerPos;

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.7f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.94f, 0.9f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.9f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.94f, 0.7f, 84.0f) + playerPos;

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.9f, 84.0f) + playerPos;
    //LR
    cub_vert_pos[idx++] = glm::vec4(0.8f, 0.91f, 0.7f, 84.0f) + playerPos;
    //LL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.7f, 84.0f) + playerPos;
    //UL
    cub_vert_pos[idx++] = glm::vec4(xEnd, 0.91f, 0.9f, 84.0f) + playerPos;
}


void createBirdVertexNormalsWings(glm::vec4 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,1,0);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(1,0,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(-1,0,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,0,-1,0);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,1,0,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec4(0,-1,0,0);
    }
}

void createBirdIndicesWings(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Pet::updateVBOdataWings(glm::vec3 p, int side)
{
    glm::vec4 playerPos(p.x + 1, p.y + 0.7, p.z - 1, 0.f);
    GLuint sph_idx[CUB_IDX_COUNT];
    glm::vec4 sph_vert_pos[CUB_VERT_COUNT];
    glm::vec4 sph_vert_nor[CUB_VERT_COUNT];
    glm::vec4 sph_vert_col[CUB_VERT_COUNT];

    for(int i = 0; i < CUB_VERT_COUNT; ++i) {
        sph_vert_col[i] = glm::vec4(1.f/16.f, 7.f/16.f, 0, 0);
    }

    createBirdVertexPositionsWings(sph_vert_pos, playerPos, side);
    createBirdVertexNormalsWings(sph_vert_nor);
    createBirdIndicesWings(sph_idx);

    indexCounts[INDEX] = CUB_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateBuffer(INDEX);
    bindBuffer(INDEX);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(GLuint), sph_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_pos, GL_STATIC_DRAW);

    generateBuffer(NORMAL);
    bindBuffer(NORMAL);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_nor, GL_STATIC_DRAW);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_col, GL_STATIC_DRAW);

}

void Pet::createVBOdata()
{
    // GLuint sph_idx[CUB_IDX_COUNT];
    // glm::vec4 playerPos(128.f, 150.f, 128.f, 0.f);
    // glm::vec4 sph_vert_pos[CUB_VERT_COUNT];
    // glm::vec4 sph_vert_nor[CUB_VERT_COUNT];
    // glm::vec4 sph_vert_col[CUB_VERT_COUNT];

    // for(int i = 0; i < CUB_VERT_COUNT; ++i) {
    //     sph_vert_col[i] = glm::vec4(1.f, 0.f, 0.f, 0.f);
    // }

    // createBirdVertexPositionsWings(sph_vert_pos, playerPos);
    // createBirdVertexNormalsWings(sph_vert_nor);
    // createBirdIndicesWings(sph_idx);

    // indexCounts[INDEX] = CUB_IDX_COUNT;

    // // Create a VBO on our GPU and store its handle in bufIdx
    // generateBuffer(INDEX);
    // bindBuffer(INDEX);
    // // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    // mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUB_IDX_COUNT * sizeof(GLuint), sph_idx, GL_STATIC_DRAW);

    // // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // // array buffers rather than element array buffers, as they store vertex attributes like position.
    // generateBuffer(POSITION);
    // bindBuffer(POSITION);
    // mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_pos, GL_STATIC_DRAW);

    // generateBuffer(NORMAL);
    // bindBuffer(NORMAL);
    // mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_nor, GL_STATIC_DRAW);

    // generateBuffer(COLOR);
    // bindBuffer(COLOR);
    // mp_context->glBufferData(GL_ARRAY_BUFFER, CUB_VERT_COUNT * sizeof(glm::vec4), sph_vert_col, GL_STATIC_DRAW);

}

void Pet::draw(ShaderProgram* shader, glm::vec3 p) {
    updateVBOdataBody(p);
    shader->draw(*this);
    // Left
    updateVBOdataWings(p, 0);
    shader->draw(*this);
    // Right
    updateVBOdataWings(p, 1);
    shader->draw(*this);
}

// void Pet::updateVBOdata(glm::vec3 p)
// {
//     glm::vec4 playerPos(p.x, p.y, p.z, 0.f);
//     GLuint idx[PET_IDX_COUNT] = {0, 1, 2, 0, 2, 3};
//     // glm::vec4 playerPos(128.f, 150.f, 128.f, 0.f);
//     glm::vec4 vert_pos[PET_VERT_COUNT] {glm::vec4(-1.f, -1.f, 1.f, 1.f) + playerPos,
//                                        glm::vec4(1.f, -1.f, 1.f, 1.f) + playerPos,
//                                        glm::vec4(1.f, 1.f, 1.f, 1.f) + playerPos,
//                                        glm::vec4(-1.f, 1.f, 1.f, 1.f) + playerPos};
//     glm::vec4 vert_col[PET_VERT_COUNT] {glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f)};


//     indexCounts[INDEX] = PET_IDX_COUNT;

//     // Create a VBO on our GPU and store its handle in bufIdx
//     generateBuffer(INDEX);
//     bindBuffer(INDEX);
//     // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
//     // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
//     mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, PET_IDX_COUNT * sizeof(GLuint), idx, GL_STATIC_DRAW);

//     // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
//     // array buffers rather than element array buffers, as they store vertex attributes like position.
//     generateBuffer(POSITION);
//     bindBuffer(POSITION);
//     mp_context->glBufferData(GL_ARRAY_BUFFER, PET_VERT_COUNT * sizeof(glm::vec4), vert_pos, GL_STATIC_DRAW);

//     generateBuffer(COLOR);
//     bindBuffer(COLOR);
//     mp_context->glBufferData(GL_ARRAY_BUFFER, PET_VERT_COUNT * sizeof(glm::vec4), vert_col, GL_STATIC_DRAW);

// }

// void Pet::createVBOdata()
// {

//     GLuint idx[PET_IDX_COUNT] = {0, 1, 2, 0, 2, 3};
//     glm::vec4 playerPos(128.f, 150.f, 128.f, 0.f);
//     glm::vec4 vert_pos[PET_VERT_COUNT] {glm::vec4(-1.f, -1.f, 1.f, 1.f) + playerPos,
//                            glm::vec4(1.f, -1.f, 1.f, 1.f) + playerPos,
//                            glm::vec4(1.f, 1.f, 1.f, 1.f) + playerPos,
//                            glm::vec4(-1.f, 1.f, 1.f, 1.f) + playerPos};
//     glm::vec4 vert_col[PET_VERT_COUNT] {glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f),
//                                        glm::vec4(1.f, 0.f, 0.f, 0.f)};


//     indexCounts[INDEX] = PET_IDX_COUNT;

//     // Create a VBO on our GPU and store its handle in bufIdx
//     generateBuffer(INDEX);
//     bindBuffer(INDEX);
//     // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
//     // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
//     mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, PET_IDX_COUNT * sizeof(GLuint), idx, GL_STATIC_DRAW);

//     // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
//     // array buffers rather than element array buffers, as they store vertex attributes like position.
//     generateBuffer(POSITION);
//     bindBuffer(POSITION);
//     mp_context->glBufferData(GL_ARRAY_BUFFER, PET_VERT_COUNT * sizeof(glm::vec4), vert_pos, GL_STATIC_DRAW);

//     generateBuffer(COLOR);
//     bindBuffer(COLOR);
//     mp_context->glBufferData(GL_ARRAY_BUFFER, PET_VERT_COUNT * sizeof(glm::vec4), vert_col, GL_STATIC_DRAW);

// }
