#pragma once
#include <pipeline/passes/DeferredShadingPass.h>
#include <scene/Volume.h>
#include "scene/Mesh.h"
#include "scene/Camera.h"
#include "passes/OpaquePass.h"
#include "passes/VolumePass.h"
#include "passes/PostprocessPass.h"


class RenderPipeline {
public:
    RenderPipeline();

    void run();


    inline void setMeshes(const std::shared_ptr<std::vector<Mesh>>& meshes) {
        opaque_.setMeshes(meshes);
    }

    inline void setVolumes(const std::shared_ptr<Volume>& volumes) {
        volume_.setVolumes(volumes);
        deferred_.setVolumes(volumes);
    }

    inline void setCamera(const std::shared_ptr<Camera>& camera) {
        opaque_.setCamera(camera);
        volume_.setCamera(camera);
        deferred_.setCamera(camera);
    }

private:
    OpaquePass opaque_;
    VolumePass volume_;
    DeferredShadingPass deferred_;
    PostprocessPass postprocess_;

    std::shared_ptr<Framebuffer> final_fb_ {std::make_shared<Framebuffer>()};

};


