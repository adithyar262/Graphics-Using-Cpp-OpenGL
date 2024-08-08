#ifndef CAMERA_H
#define CAMERA_H
#if 1
#include <glm/glm.hpp>
//A perspective projection camera
class Camera
{
    glm::vec4 pos;
    glm::vec4 forward;
    glm::vec4 up;
    glm::vec4 right;
    glm::vec4 target;
    float fovy;
    float aspect;
    float nearClip;
    float farClip;

public:
    Camera();
    // Camera(unsigned int w, unsigned int h);

    // void recomputeAspectRatio(unsigned int w, unsigned int h);

    // glm::mat4 getViewProj();
    // glm::mat4 getViewProj_OrientOnly();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec4 getForwardVector();

    // void RotateAboutGlobalUp(float deg);
    // void RotateAboutLocalRight(float deg);

    void TranslateForward(float amt);
    void TranslateRight(float amt);
    void TranslateUp(float z);

    void rotateRight(float angle);
    void rotateUp(float angle);
    void rotateForward(float angle);

    // void Zoom(float amt);
};
#endif
#if 0
#include <glm/glm.hpp>

class Camera
{
private:
    glm::vec4 forward = glm::vec4(0, 0, -1, 0);
    glm::vec4 right = glm::vec4(1, 0, 0, 0);
    glm::vec4 up = glm::vec4(0, 1, 0, 0);
    int field_view = 45;
    glm::vec4 eye = glm::vec4(0, 0, 10, 1);
    float near_plane = 0.01;
    float far_plane = 100.0;
    float aspect_ratio = 1.0;
    float tan_angle = tan(field_view * M_PI * 1.0 / (180.0 * 2.0));
public:
    glm::mat4 ViewMatrix();
    glm::mat4 ProjectionMatrix();
    void x_translate(float displacement);
    void y_translate(float displacement);
    void z_translate(float displacement);
    void x_rotate(int angle);
    void y_rotate(int angle);
    void z_rotate(int angle);
};
#endif
#endif //CAMERA_H
