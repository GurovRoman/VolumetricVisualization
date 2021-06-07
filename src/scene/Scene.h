#pragma once
#include <ui/GUI.h>
#include "GLcommon.h"
#include "Mesh.h"
#include "Camera.h"
#include "gl/Program.h"
#include "pipeline/RenderPipeline.h"
#include "Volume.h"


class Scene : public glfwm::Drawable {
public:
    Scene();
    void draw(glfwm::WindowID id) override;

    inline auto& getCamera() const {
        return camera_;
    }
    inline auto& getMeshes() const {
        return meshes_;
    }
    inline auto& getVolumes() const {
        return volumes_;
    }

private:
    std::shared_ptr<std::vector<std::shared_ptr<Mesh>>> meshes_ {std::make_shared<std::vector<std::shared_ptr<Mesh>>>()};
    std::shared_ptr<Camera> camera_ {std::make_shared<Camera>()};
    std::shared_ptr<Volume> volumes_ {std::make_shared<Volume>()};

    RenderPipeline pipeline_;
    GUI gui_;
};
