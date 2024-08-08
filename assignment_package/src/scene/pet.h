#pragma once

#include "drawable.h"
#include "shaderprogram.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Pet : public Drawable
{
public:
    Pet(OpenGLContext* context);
    virtual void createVBOdata();
    void updateVBOdataBody(glm::vec3 p);
    void updateVBOdataWings(glm::vec3 p, int side);
    void draw(ShaderProgram* shader, glm::vec3 p);
};
