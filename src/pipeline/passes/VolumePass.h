#pragma once

#include <scene/Volume.h>
#include "pipeline/RenderPass.h"
#include "gl/Program.h"
#include "scene/Mesh.h"
#include "scene/Camera.h"
#include "gl/Texture.h"


class VolumePass : public RenderPass {
public:
    VolumePass();
    void run() override;

    inline void setDepthTex(const TexturePtr& id) {
        gDepth_tex_ = id;
    }

    inline void setVolumes(const std::shared_ptr<Volume>& volumes) {
        volumes_ = volumes;
    }

private:
    Program volume_program_;


    TexturePtr gDepth_tex_;
    std::shared_ptr<Volume> volumes_;

};


