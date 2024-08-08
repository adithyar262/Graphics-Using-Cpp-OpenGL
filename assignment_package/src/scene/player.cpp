#include "player.h"
#include <QString>
#include <iostream>
#include <math.h>

Player::Player(glm::vec3 pos, const Terrain &terrain)
    : Entity(pos), m_velocity(0,0,0), m_acceleration(0,0,0),
    m_camera(pos + glm::vec3(0, 1.5f, 0)), mcr_terrain(terrain),
    flightMode(true),
    mcr_camera(m_camera)
{}

Player::~Player()
{}

void Player::tick(float dT, InputBundle &input) {
    // std::string str("( " + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + ")");
    // std::cout << "============================" << str << std::endl;
    processInputs(input);
    computePhysics(dT, mcr_terrain);
}

void Player::updateSelectedBlock() {
    // all blocks EMPTY, GRASS, DIRT, STONE, WATER, SNOW, SAND, LAVA, BEDROCK
    if (currentSelectBlock == GRASS) {
        currentSelectBlock = DIRT;
        return;
    }
    if (currentSelectBlock == DIRT) {
        currentSelectBlock = STONE;
        return;
    }
    if (currentSelectBlock == STONE) {
        currentSelectBlock = WATER;
        return;
    }
    if (currentSelectBlock == WATER) {
        currentSelectBlock = SNOW;
        return;
    }
    if (currentSelectBlock == SNOW) {
        currentSelectBlock = SAND;
        return;
    }
    if (currentSelectBlock == SAND) {
        currentSelectBlock = LAVA;
        return;
    }
    if (currentSelectBlock == LAVA) {
        currentSelectBlock = BEDROCK;
        return;
    }
    if (currentSelectBlock == BEDROCK) {
        currentSelectBlock = GRASS;
        return;
    }

}

void Player::processInputs(InputBundle &inputs) {
    if (inputs.iPressed) {
        // update the player selected block
        updateSelectedBlock();
        inputs.iPressed = false;
    }
    // Update the Player's velocity and acceleration based on the state of the inputs.
    if (inputs.fPressed) {
        if (flightMode) {
            while (checkCollision()) moveUpGlobal(1.0f);
            m_velocity = glm::vec3(0);
        }
        flightMode = !flightMode;
        inputs.fPressed = false;
    }

    m_acceleration = glm::vec3(0.0f);

    float moveSpeed = inputs.shiftPressed ? 60.0f : 20.0f;
    float jumpForce = inputs.shiftPressed ? 15.0f : 5.0f;
    float rotateSpeed = inputs.shiftPressed ? 3.0f : 1.0f;

    if (inputs.upPressed) rotateOnRightLocal(rotateSpeed);
    if (inputs.downPressed) rotateOnRightLocal(-rotateSpeed);
    if (inputs.leftPressed) rotateOnUpGlobal(rotateSpeed);
    if (inputs.rightPressed) rotateOnUpGlobal(-rotateSpeed);

    // if (inputs.n1Pressed) Later;
    // if (inputs.n2Pressed) Later;

    if (flightMode) {
        if (inputs.wPressed) m_acceleration += m_forward * moveSpeed;
        if (inputs.sPressed) m_acceleration -= m_forward * moveSpeed;
        if (inputs.dPressed) m_acceleration += m_right * moveSpeed;
        if (inputs.aPressed) m_acceleration -= m_right * moveSpeed;
        if (inputs.ePressed) m_acceleration += m_up * moveSpeed;
        if (inputs.qPressed) m_acceleration -= m_up * moveSpeed;
    } else {
        glm::vec3 forwardFlat = glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));
        glm::vec3 rightFlat = glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z));

        if (inputs.wPressed) m_acceleration += forwardFlat * moveSpeed;
        if (inputs.sPressed) m_acceleration -= forwardFlat * moveSpeed;
        if (inputs.dPressed) m_acceleration += rightFlat * moveSpeed;
        if (inputs.aPressed) m_acceleration -= rightFlat * moveSpeed;

        glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        m_acceleration += gravity;

        if (inputs.spacePressed) {
            m_velocity.y = jumpForce;
            if (!checkInFluid()) inputs.spacePressed = false;
        }
    }

    float dragFactor = 0.90f;
    m_velocity *= dragFactor;
}

glm::vec3 Player::findAdjacentBlockPosition(const glm::vec3 &hitPoint, const glm::ivec3 &hitBlock) const {
    glm::vec3 blockCenter = glm::vec3(hitBlock) + glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 direction = hitPoint - blockCenter;
    glm::vec3 normalizedDirection = glm::normalize(direction);

    // Determine the primary axis of impact
    float maxComponent = std::max({abs(normalizedDirection.x), abs(normalizedDirection.y), abs(normalizedDirection.z)});
    glm::ivec3 adjacentBlock = hitBlock;

    if(maxComponent == abs(normalizedDirection.x)) {
        adjacentBlock.x += normalizedDirection.x > 0 ? 1 : -1;
    } else if(maxComponent == abs(normalizedDirection.y)) {
        adjacentBlock.y += normalizedDirection.y > 0 ? 1 : -1;
    } else if(maxComponent == abs(normalizedDirection.z)) {
        adjacentBlock.z += normalizedDirection.z > 0 ? 1 : -1;
    }

    return glm::vec3(adjacentBlock);
}

void Player::blockOpreation(bool flag, Terrain &terrain, BlockType selectedBlock) {
    if (flightMode) {
        // don't add block in fly mode
        return;
    }
    glm::vec3 rayDirection = glm::normalize(m_forward);
    glm::vec3 rayOrigin = m_position + glm::vec3(0.0f, 1.5f, 0.0f);
    float maxDistance = 3.0f;
    float out_dist;
    glm::ivec3 out_blockHit;

    if (rayMarch(rayOrigin, maxDistance * rayDirection, terrain, &out_dist, &out_blockHit) && out_dist <= maxDistance) {
        // std::cout << "Collision detected at distance " << out_dist << std::endl;
        if(!flag) {
            if (out_blockHit.y == 0) return; // case for bedrock
            terrain.setGlobalBlockAt(out_blockHit.x, out_blockHit.y, out_blockHit.z, BlockType::EMPTY);
        } else {
            glm::vec3 adjacentPosition = findAdjacentBlockPosition(rayOrigin + rayDirection * out_dist, out_blockHit);
            terrain.setGlobalBlockAt(adjacentPosition.x, adjacentPosition.y, adjacentPosition.z, selectedBlock);
            if (!flightMode) while (checkCollision()) moveUpGlobal(1.0f);
        }
    }
}

float Player::checkAxisMovement(const glm::vec3 &axis, float distance, const Terrain &terrain) {
    glm::vec3 direction = axis * distance;
    float maxLength = glm::length(direction);

    float closestCollisionDistance = maxLength;
    for (int corner = 0; corner < 12; ++corner) {
        glm::vec3 cornerOrigin = m_position + getCornerOffset(corner);
        float out_dist = 3.0f;
        glm::ivec3 out_blockHit;

        if (rayMarch(cornerOrigin, direction, terrain, &out_dist, &out_blockHit)) {
            closestCollisionDistance = glm::min(closestCollisionDistance, out_dist);

        }
    }
    // std::cout << "Allowed movement: " << closestCollisionDistance << std::endl;
    return closestCollisionDistance;
}

bool Player::checkCollision(){
    for (int i = 0; i < 12; ++i){
        glm::vec3 pos = m_position + getCornerOffset(i);
        glm::ivec3 currentBlockPos = glm::ivec3(glm::floor(pos));
        auto block = mcr_terrain.getGlobalBlockAt(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z);
        if ( block != BlockType::EMPTY && block != BlockType::WATER && block != BlockType::LAVA) return true;
    }
    return false;
}

bool Player::checkInFluid(){
    for (int i = 0; i < 12; ++i){
        glm::vec3 pos = m_position + getCornerOffset(i);
        glm::ivec3 currentBlockPos = glm::ivec3(glm::floor(pos));
        auto block = mcr_terrain.getGlobalBlockAt(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z);
        if (block == BlockType::WATER || block == BlockType::LAVA) return true;
    }
    return false;
}

BlockType Player::cameraIn(){
    glm::vec3 pos = mcr_camera.mcr_position;
    glm::ivec3 currentBlockPos = glm::ivec3(glm::floor(pos));
    return mcr_terrain.getGlobalBlockAt(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z);
}

bool Player::rayMarch(const glm::vec3 &rayOrigin, const glm::vec3 &direction, const Terrain &terrain, float *out_dist, glm::ivec3 *out_blockHit) const {
    if (glm::length(direction) == 0) {
        return false;
    }

    float maxDistance = glm::length(direction);
    glm::vec3 normalizedDirection = glm::normalize(direction);

    const float minStepSize = 0.002f;
    glm::vec3 currentPos = rayOrigin;
    float currentDist = 0.0f;

    while (currentDist < maxDistance) {
        glm::ivec3 currentBlockPos = glm::ivec3(glm::floor(currentPos));

        BlockType bt = terrain.getGlobalBlockAt(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z);

        if (bt != BlockType::EMPTY && bt != BlockType::WATER && bt != BlockType::LAVA) {
            // std::string str("( " + std::to_string(currentBlockPos.x) + ", " + std::to_string(currentBlockPos.y) + ", " + std::to_string(currentBlockPos.z) + ")");
            // std::cout << "============================" << str << std::endl;
            *out_dist = currentDist;
            *out_blockHit = currentBlockPos;
            return true;
        }

        glm::vec3 toNextBoundary = glm::vec3(
            normalizedDirection.x > 0 ? ceil(currentPos.x) - currentPos.x : currentPos.x - floor(currentPos.x),
            normalizedDirection.y > 0 ? ceil(currentPos.y) - currentPos.y : currentPos.y - floor(currentPos.y),
            normalizedDirection.z > 0 ? ceil(currentPos.z) - currentPos.z : currentPos.z - floor(currentPos.z)
            );


        for (int i = 0; i < 3; ++i) {
            if (toNextBoundary[i] == 0) toNextBoundary[i] = maxDistance;
            else toNextBoundary[i] /= fabs(normalizedDirection[i]);
        }

        float minT = glm::min(toNextBoundary.x, glm::min(toNextBoundary.y, toNextBoundary.z));

        float stepSize = glm::min(minT + 0.001f, minStepSize);
        currentPos += normalizedDirection * stepSize;
        currentDist += stepSize;

        if (currentDist > maxDistance) {
            *out_dist = maxDistance;
            return false;
        }
    }

    *out_dist = currentDist;
    return false;
}

glm::vec3 Player::getCornerOffset(int cornerIndex) const {
    float halfSize = 0.5f;
    float height = 1.0f;
    switch (cornerIndex) {
    // Bottom block corners
    case 0: return glm::vec3(-halfSize, 0, -halfSize);
    case 1: return glm::vec3(halfSize, 0, -halfSize);
    case 2: return glm::vec3(-halfSize, 0, halfSize);
    case 3: return glm::vec3(halfSize, 0, halfSize);
    // Top block corners (2 units above the bottom block to account for the player's 'head')
    case 4: return glm::vec3(-halfSize, height, -halfSize);
    case 5: return glm::vec3(halfSize, height, -halfSize);
    case 6: return glm::vec3(-halfSize, height, halfSize);
    case 7: return glm::vec3(halfSize, height, halfSize);

    case 8: return glm::vec3(-halfSize, height * 2, -halfSize);
    case 9: return glm::vec3(halfSize, height * 2, -halfSize);
    case 10: return glm::vec3(-halfSize, height * 2, halfSize);
    case 11: return glm::vec3(halfSize, height * 2, halfSize);

    default: return glm::vec3(0);
    }
}

void Player::computePhysics(float dT, const Terrain &terrain) {
    // TODO: Update the Player's position based on its acceleration and velocity, and also perform collision detection.
    // std::cout << "x: " << m_position.x << ", y: " << m_position.y << ", z: " << m_position.z << std::endl;

    m_velocity += m_acceleration * dT;
    if (!flightMode && checkInFluid()) m_velocity *= 0.66f;
    glm::vec3 potentialMovement = m_velocity * dT;
    glm::vec3 allowedMovement(0);

    for (int i = 0; i < 3; ++i) {
        if (m_velocity[i] != 0 && !flightMode) {
            // std::cout << "Checking " << i << std::endl;
            glm::vec3 axis = i == 0 ? glm::vec3(1, 0, 0) : (i == 1 ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1));
            float axisMovement = checkAxisMovement(axis, potentialMovement[i], terrain);
            if (axisMovement != 0 && !isnan(axisMovement)) {
                float temp = floor(abs(axisMovement) * 100);
                allowedMovement[i] = potentialMovement[i] > 0 ? (temp / 100) : (-temp / 100);
            }
        }
    }

    if (flightMode) allowedMovement += potentialMovement;
    // moveAlongVector(allowedMovement);
    moveForwardGlobal(allowedMovement.z);
    moveRightGlobal(allowedMovement.x);
    moveUpGlobal(allowedMovement.y);

}

void Player::setCameraWidthHeight(unsigned int w, unsigned int h) {
    m_camera.setWidthHeight(w, h);
}

void Player::moveAlongVector(glm::vec3 dir) {
    Entity::moveAlongVector(dir);
    m_camera.moveAlongVector(dir);
}
void Player::moveForwardLocal(float amount) {
    Entity::moveForwardLocal(amount);
    m_camera.moveForwardLocal(amount);
}
void Player::moveRightLocal(float amount) {
    Entity::moveRightLocal(amount);
    m_camera.moveRightLocal(amount);
}
void Player::moveUpLocal(float amount) {
    Entity::moveUpLocal(amount);
    m_camera.moveUpLocal(amount);
}
void Player::moveForwardGlobal(float amount) {
    Entity::moveForwardGlobal(amount);
    m_camera.moveForwardGlobal(amount);
}
void Player::moveRightGlobal(float amount) {
    Entity::moveRightGlobal(amount);
    m_camera.moveRightGlobal(amount);
}
void Player::moveUpGlobal(float amount) {
    Entity::moveUpGlobal(amount);
    m_camera.moveUpGlobal(amount);
}
void Player::rotateOnForwardLocal(float degrees) {
    Entity::rotateOnForwardLocal(degrees);
    m_camera.rotateOnForwardLocal(degrees);
}
void Player::rotateOnRightLocal(float degrees) {
    Entity::rotateOnRightLocal(degrees);
    m_camera.rotateOnRightLocal(degrees);
}
void Player::rotateOnUpLocal(float degrees) {
    Entity::rotateOnUpLocal(degrees);
    m_camera.rotateOnUpLocal(degrees);
}
void Player::rotateOnForwardGlobal(float degrees) {
    Entity::rotateOnForwardGlobal(degrees);
    m_camera.rotateOnForwardGlobal(degrees);
}
void Player::rotateOnRightGlobal(float degrees) {
    Entity::rotateOnRightGlobal(degrees);
    m_camera.rotateOnRightGlobal(degrees);
}
void Player::rotateOnUpGlobal(float degrees) {
    Entity::rotateOnUpGlobal(degrees);
    m_camera.rotateOnUpGlobal(degrees);
}

QString Player::posAsQString() const {
    std::string str("( " + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + ")");
    return QString::fromStdString(str);
}
QString Player::velAsQString() const {
    std::string str("( " + std::to_string(m_velocity.x) + ", " + std::to_string(m_velocity.y) + ", " + std::to_string(m_velocity.z) + ")");
    return QString::fromStdString(str);
}
QString Player::accAsQString() const {
    std::string str("( " + std::to_string(m_acceleration.x) + ", " + std::to_string(m_acceleration.y) + ", " + std::to_string(m_acceleration.z) + ")");
    return QString::fromStdString(str);
}
QString Player::lookAsQString() const {
    std::string str("( " + std::to_string(m_forward.x) + ", " + std::to_string(m_forward.y) + ", " + std::to_string(m_forward.z) + ")");
    return QString::fromStdString(str);
}
