#include "squareplane.h"
#include<la.h>
#include <iostream>

SquarePlane::SquarePlane(OpenGLContext *mp_context) : Drawable(mp_context)
{}

SquarePlane::~SquarePlane()
{}

void SquarePlane::initializeAndBufferGeometryData()
{

    std::vector<glm::vec3> pos {glm::vec3(-2, -2, 0),
                                glm::vec3(2, -2, 0),
                                glm::vec3(2, 2, 0)};

    std::vector<glm::vec3> nor {glm::vec3(0, 0, 1),
                                glm::vec3(0, 0, 1),
                                glm::vec3(0, 0, 1)};

    std::vector<glm::vec3> col {glm::vec3(0.5, 0, 0),
                                glm::vec3(0, 0.5, 0),
                                glm::vec3(0, 0, 0.5)};

    std::vector<GLuint> idx {0, 1, 2};

    indexBufferLength = 3;

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
