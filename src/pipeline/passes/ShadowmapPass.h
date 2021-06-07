#pragma once
#include <pipeline/RenderPass.h>


class ShadowmapPass : public RenderPass {
public:
    ShadowmapPass();
    void run() override;

    inline const TexturePtr& getShadowmap() {
        return shadowmap_;
    }

    inline void setMeshes(const std::shared_ptr<std::vector<std::shared_ptr<Mesh>>>& meshes) {
        meshes_ = meshes;
    }

    static inline const GLint shadowmap_resolution = 2048;

private:
    Program mesh_program_;
    TexturePtr shadowmap_;
    Framebuffer draw_buffer_;


    std::shared_ptr<std::vector<std::shared_ptr<Mesh>>> meshes_;
};
