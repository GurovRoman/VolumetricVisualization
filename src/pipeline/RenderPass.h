#pragma once


#include <gl/Texture.h>
#include "pipeline/RenderPass.h"
#include "gl/Program.h"
#include "gl/Framebuffer.h"
#include "scene/Mesh.h"
#include "scene/Camera.h"

class RenderPass {
protected:
    std::shared_ptr<Framebuffer> output_fb_ {Framebuffer::getDefaultFramebuffer()};
    std::shared_ptr<Camera> camera_ {std::make_shared<Camera>()};
public:
    virtual void run() = 0;

    inline void setOutputFramebuffer(const std::shared_ptr<Framebuffer>& fb) {
        output_fb_ = fb;
    }

    inline void setCamera(const std::shared_ptr<Camera>& camera) {
        camera_ = camera;
    }
};


