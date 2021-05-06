#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


class Transform {
public:
    glm::mat4x4 getTransformMatrix() const;
    glm::mat4x4 getRotationMatrix() const;

public:
    glm::vec3 pos {0};
    glm::vec3 angle {0};  // Pitch, Yaw, Roll
    glm::vec3 scale {1};
};


