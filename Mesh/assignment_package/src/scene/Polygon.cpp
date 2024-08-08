#include "Polygon.h"
#include<la.h>
#include <iostream>

Polygon::Polygon(OpenGLContext *mp_context) : Drawable(mp_context)
{}

// Polygon::~Polygon()
// {}

void Polygon::initializeAndBufferGeometryData()
{

    std::vector<glm::vec3> pos {glm::vec3(-2, -2, 0),
                               glm::vec3(2, -2, 0),
                               glm::vec3(2, 2, 0),
                               glm::vec3(-2, 2, 0)};

    std::vector<glm::vec3> nor {glm::vec3(0, 0, 1),
                               glm::vec3(0, 0, 1),
                               glm::vec3(0, 0, 1),
                               glm::vec3(0, 0, 1)};

    std::vector<glm::vec3> col {glm::vec3(1, 0, 0),
                               glm::vec3(0, 1, 0),
                               glm::vec3(0, 0, 1),
                               glm::vec3(1, 1, 0)};

    std::vector<GLuint> idx {0, 1, 2, 0, 2, 3};

    indexBufferLength = 6;

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    generateBuffer(NORMAL);
    bindBuffer(NORMAL);
    bufferData(NORMAL, nor);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);

}

static const int PNT_IDX_COUNT = 1;
static const int PNT_VERT_COUNT = 1;


DrawPoint::DrawPoint(OpenGLContext *context) : Drawable(context)
{
    mode = GL_POINTS;
}


void DrawPoint::initializeAndBufferGeometryData()
{
    int size = pos.size();

    // GLuint pnt_idx[size];
    std::vector<GLuint> idx;

    for (int i = 0; i < size; i++)
    {
        idx.push_back(i);
        col.push_back(glm::vec4(1,1,1,1));
    }

    indexBufferLength = size;

    // std::vector<glm::vec3> pos {glm::vec3(-2, -2, 0),
    //                            glm::vec3(2, -2, 0),
    //                            glm::vec3(2, 2, 0),
    //                            glm::vec3(-2, 2, 0)};

    // std::vector<glm::vec3> nor {glm::vec3(0, 0, 1),
    //                            glm::vec3(0, 0, 1),
    //                            glm::vec3(0, 0, 1),
    //                            glm::vec3(0, 0, 1)};

    // std::vector<glm::vec3> col {glm::vec3(1, 0, 0),
    //                            glm::vec3(0, 1, 0),
    //                            glm::vec3(0, 0, 1),
    //                            glm::vec3(1, 1, 0)};

    // std::vector<GLuint> idx {0, 1, 2, 0, 2, 3};
    // indexBufferLength = 6;

    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // generateBuffer(NORMAL);
    // bindBuffer(NORMAL);
    // bufferData(NORMAL, nor);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);

    // count = size;

    // // Create a VBO on our GPU and store its handle in bufIdx
    // generateIdx();
    // // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // // and that it will be treated as an element array buffer (since it will contain triangle indices)
    // context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    // context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), pnt_idx, GL_STATIC_DRAW);

    // // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // // array buffers rather than element array buffers, as they store vertex attributes like position.
    // generatePos();
    // context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    // context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &pos[0], GL_STATIC_DRAW);

    // generateCol();
    // context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    // context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), &col[0], GL_STATIC_DRAW);
}

static const int LNE_IDX_COUNT = 2;//760 tris * 3
static const int LNE_VERT_COUNT = 2;

DrawLine::DrawLine(OpenGLContext *context) : Drawable(context)
{
    mode = GL_LINES;
}

void DrawLine::initializeAndBufferGeometryData()
{
    int size = pos.size();

    std::vector<GLuint> idx;

    for (int i = 0; i < size; i++)
    {
        idx.push_back(i);
        if (i % 2)
            col.push_back(glm::vec4(1,1,0,1));
        else
            col.push_back(glm::vec4(1,0,0,1));
    }

    indexBufferLength = size;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // generateBuffer(NORMAL);
    // bindBuffer(NORMAL);
    // bufferData(NORMAL, nor);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);
}

DrawFace::DrawFace(OpenGLContext *context) : Drawable(context)
{
    mode = GL_LINES;
}

void DrawFace::initializeAndBufferGeometryData()
{
    int size = 2 * pos.size();
    std::vector<GLuint> idx;

    //    printf("DrawFace size:%d\n", size);

    GLuint fce_idx[size];

    for (int i = 0; i < size; i++)
    {
        idx.push_back(i);
    }

    indexBufferLength = size * 2;

    // Create a VBO on our GPU and store its handle in bufIdx
    generateBuffer(INDEX);
    bindBuffer(INDEX);
    bufferData(INDEX, idx);

    generateBuffer(POSITION);
    bindBuffer(POSITION);
    bufferData(POSITION, pos);

    // generateBuffer(NORMAL);
    // bindBuffer(NORMAL);
    // bufferData(NORMAL, nor);

    generateBuffer(COLOR);
    bindBuffer(COLOR);
    bufferData(COLOR, col);

}


