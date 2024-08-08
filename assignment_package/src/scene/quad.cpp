#include "quad.h"

#define QUAD_IDX_COUNT 6
#define QUAD_VERT_COUNT 4

Quad::Quad(OpenGLContext *context) : Drawable(context)
{}

void Quad::createVBOdata()
{
    GLuint idx[QUAD_IDX_COUNT] = {0, 1, 2, 0, 2, 3};
    glm::vec4 vert_pos[QUAD_VERT_COUNT] {glm::vec4(-1.f, -1.f, 1.f, 1.f),
                           glm::vec4(1.f, -1.f, 1.f, 1.f),
                           glm::vec4(1.f, 1.f, 1.f, 1.f),
                           glm::vec4(-1.f, 1.f, 1.f, 1.f)};


    indexCounts[INDEX] = QUAD_IDX_COUNT;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateBuffer(INDEX);
    bindBuffer(INDEX);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, QUAD_IDX_COUNT * sizeof(GLuint), idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generateBuffer(POSITION);
    bindBuffer(POSITION);
    mp_context->glBufferData(GL_ARRAY_BUFFER, QUAD_VERT_COUNT * sizeof(glm::vec4), vert_pos, GL_STATIC_DRAW);

}
