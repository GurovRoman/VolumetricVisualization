#pragma once
#include "GLcommon.h"

extern size_t frame_counter;

class Scene : public glfwm::Drawable {
public:
    void draw(const glfwm::WindowID id) override;
};
