#pragma once

#include "drawable.h"
#include "shaderprogram.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

typedef enum : unsigned char {
    RAIN_INITIATE= 0,
    RAIN_START,
    RAIN_INPROGRESS,
    RAIN_STOP
}RainStatus_t;

class Weather : public Drawable
{
private:
    RainStatus_t rainStatus = RAIN_INITIATE;
    bool typeRain = true; // false = snow
    int rainDuration = -1;
    int delay = -1;
    float currTick;
public:
    Weather(OpenGLContext* context);
    virtual void createVBOdata();
    void updateRain(glm::vec3 p);
    void draw(ShaderProgram* shader, glm::vec3 p, float time);
};
