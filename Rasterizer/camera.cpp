#include "camera.h"

Camera::Camera(void)
    : pos(0, 0, 10, 1), forward(0, 0, -1, 0), up(0, 1, 0, 0), right(1, 0, 0, 0), target(0, 0, 0, 0),
    fovy(45), aspect(1.0), nearClip(0.01), farClip(100.0)
{}

glm::mat4 Camera::getViewMatrix() {

    glm::mat4 O_matrix, T_matrix;

    glm::vec4 o_1 = {right[0], up[0], forward[0], 0};
    glm::vec4 o_2 = {right[1], up[1], forward[1], 0};
    glm::vec4 o_3 = {right[2], up[2], forward[2], 0};
    glm::vec4 o_4 = {0, 0, 0, 1};
    O_matrix = {o_1, o_2, o_3, o_4};

    glm::vec4 m_1 = {1, 0, 0, 0};
    glm::vec4 m_2 = {0, 1, 0, 0};
    glm::vec4 m_3 = {0, 0, 1, 0};
    glm::vec4 m_4 = {-1*pos[0], -1*pos[1], -1*pos[2], 1};
    T_matrix = {m_1, m_2, m_3, m_4};

    glm::mat4 OriginalMatrix = O_matrix * T_matrix;

    return OriginalMatrix;
}

glm::vec4 Camera::getForwardVector() {
    return forward;
}

glm::mat4 Camera::getProjectionMatrix() {
    float tan_angle = tan(glm::radians(fovy) / 2.0f);

    glm::vec4 p_1 = {1 / (aspect * tan_angle), 0, 0, 0};
    glm::vec4 p_2 = {0, 1 / tan_angle, 0, 0};
    glm::vec4 p_3 = {0, 0, farClip / (farClip - nearClip), 1};
    glm::vec4 p_4 = {0, 0, (float)(-1.0 * farClip * nearClip / (farClip - nearClip)), 0};

    return {p_1, p_2, p_3, p_4};
}

void Camera::rotateForward(float angle) {
    glm::mat4 rotationMatrix(1.0f);
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    rotationMatrix[1][1] = cosTheta;
    rotationMatrix[1][2] = -sinTheta;
    rotationMatrix[2][1] = sinTheta;
    rotationMatrix[2][2] = cosTheta;

    target = (rotationMatrix * (target - pos)) + pos;
    up = rotationMatrix * up;
}

void Camera::rotateUp(float angle) {
    glm::mat4 rotationMatrix(1.0f);
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    rotationMatrix[0][0] = cosTheta;
    rotationMatrix[0][2] = sinTheta;
    rotationMatrix[2][0] = -sinTheta;
    rotationMatrix[2][2] = cosTheta;

    target = (rotationMatrix * (target - pos)) + pos;
    up = rotationMatrix * up;
}

void Camera::rotateRight(float angle) {
    glm::mat4 rotationMatrix(1.0f);
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    rotationMatrix[0][0] = cosTheta;
    rotationMatrix[0][1] = -sinTheta;
    rotationMatrix[1][0] = sinTheta;
    rotationMatrix[1][1] = cosTheta;

    target = (rotationMatrix * (target - pos)) + pos;
    up = rotationMatrix * up;
}

void Camera::TranslateForward(float amt) {
    glm::vec4 translation = forward * amt;
    pos += translation;
    target += translation;
}

void Camera::TranslateRight(float amt) {
    glm::vec4 translation = right * amt;
    pos += translation;
    target += translation;
}

void Camera::TranslateUp(float amt) {
    glm::vec4 translation = up * amt;
    pos += translation;
    target += translation;
}
