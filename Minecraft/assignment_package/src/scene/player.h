#pragma once
#include "entity.h"
#include "camera.h"
#include "terrain.h"
#include "pet.h"

class Player : public Entity {
private:
    glm::vec3 m_velocity, m_acceleration;
    Camera m_camera;
    const Terrain &mcr_terrain;

    bool flightMode;

    void processInputs(InputBundle &inputs);
    void computePhysics(float dT, const Terrain &terrain);

    bool rayMarch(const glm::vec3 &rayOrigin, const glm::vec3 &direction, const Terrain &terrain, float *out_dist, glm::ivec3 *out_blockHit) const;
    float checkAxisMovement(const glm::vec3 &axis, float distance, const Terrain &terrain);
    glm::vec3 getCornerOffset(int cornerIndex) const;
    glm::vec3 findAdjacentBlockPosition(const glm::vec3 &hitPoint, const glm::ivec3 &hitBlock) const;
    void updateSelectedBlock();
    friend class Pet;

public:
    // Readonly public reference to our camera
    // for easy access from MyGL
    const Camera& mcr_camera;
    BlockType currentSelectBlock = GRASS;

    Player(glm::vec3 pos, const Terrain &terrain);
    virtual ~Player() override;

    void setCameraWidthHeight(unsigned int w, unsigned int h);

    void tick(float dT, InputBundle &input) override;

    // Player overrides all of Entity's movement
    // functions so that it transforms its camera
    // by the same amount as it transforms itself.
    void moveAlongVector(glm::vec3 dir) override;
    void moveForwardLocal(float amount) override;
    void moveRightLocal(float amount) override;
    void moveUpLocal(float amount) override;
    void moveForwardGlobal(float amount) override;
    void moveRightGlobal(float amount) override;
    void moveUpGlobal(float amount) override;
    void rotateOnForwardLocal(float degrees) override;
    void rotateOnRightLocal(float degrees) override;
    void rotateOnUpLocal(float degrees) override;
    void rotateOnForwardGlobal(float degrees) override;
    void rotateOnRightGlobal(float degrees) override;
    void rotateOnUpGlobal(float degrees) override;

    // For sending the Player's data to the GUI
    // for display
    QString posAsQString() const;
    QString velAsQString() const;
    QString accAsQString() const;
    QString lookAsQString() const;

    void blockOpreation(bool flag, Terrain &terrain, BlockType selectedBlock);
    bool checkCollision();
    bool checkInFluid();
    BlockType cameraIn();
};
