#pragma once

#include <gl/Texture.h>
#include "pipeline/RenderPass.h"
#include "gl/Program.h"
#include "gl/Framebuffer.h"
#include "scene/Mesh.h"
#include "scene/Camera.h"


class OpaquePass : public RenderPass {
public:
    struct GBuffer {
        TexturePtr albedo;
        TexturePtr pos;
        TexturePtr normal;
        TexturePtr depth;
    };

public:
    OpaquePass();
    void run() override;

    inline void setMeshes(const std::shared_ptr<std::vector<std::shared_ptr<Mesh>>>& meshes) {
        meshes_ = meshes;
    }

    inline const std::shared_ptr<GBuffer>& getGBuffer() const {
        return gbuffer_;
    }

private:
    Program mesh_program_;

    Framebuffer gbuffer_fb_;
    std::shared_ptr<GBuffer> gbuffer_ {std::make_shared<GBuffer>()};

    std::shared_ptr<std::vector<std::shared_ptr<Mesh>>> meshes_;
};


