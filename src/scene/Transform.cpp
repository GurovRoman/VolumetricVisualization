#include "Transform.h"
#include "glm/gtx/transform.hpp"

glm::mat4x4 Transform::getTransformMatrix() const {
    return glm::translate(pos) * getRotationMatrix() * glm::scale(scale);
}

glm::mat4x4 Transform::getRotationMatrix() const {
    return glm::rotate(glm::radians(angle.y), glm::vec3{0, 1, 0}) * glm::rotate(glm::radians(angle.x), glm::vec3{1, 0, 0}) * glm::rotate(glm::radians(angle.z), glm::vec3{0, 0, 1});
}
