#pragma once
#include <memory>
#include <any>
#include "glm/vec3.hpp"
#include "scene/Transform.h"
#include "Actor.h"


class Camera : public Actor, public std::enable_shared_from_this<Camera> {
public:
    glm::vec3 GetCameraPixelPosition(int x, int y, int width, int height);

    glm::mat4x4 getViewProjMatrix() const;
    glm::mat4x4 getInfViewProjMatrix() const;

    void bindHandlers();

    void tick(float timeDelta);

public:
    float fov {90.};
    float aspect_ratio {16. / 9};
    bool perspective {true};


    float speed {3};

private:
    std::any input_handler;
};

