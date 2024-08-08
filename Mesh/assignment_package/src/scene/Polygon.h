#ifndef POLYGON_H
#define POLYGON_H

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Polygon : public Drawable
{
public:
    Polygon(OpenGLContext* mp_context);
    // virtual ~Polygon();
    void initializeAndBufferGeometryData() override;
};

class DrawLine : public Drawable
{
public:
    DrawLine(OpenGLContext *context);
    virtual void initializeAndBufferGeometryData();
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
};

class DrawPoint : public Drawable
{
public:
    DrawPoint(OpenGLContext *context);
    virtual void initializeAndBufferGeometryData();
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
};

class DrawFace : public Drawable
{
public:
    DrawFace(OpenGLContext *context);
    virtual void initializeAndBufferGeometryData();
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> col;
};

#endif //POLYGON
