#pragma once

#include <scene/Volume.h>
#include "OpaquePass.h"


class DeferredShadingPass : public RenderPass {
public:
    DeferredShadingPass();

    void run() override;

    void setGBuffer(const std::shared_ptr<OpaquePass::GBuffer>& gbuffer) {
        gbuffer_ = gbuffer;
    }

    inline void setVolumes(const std::shared_ptr<Volume>& volumes) {
        volumes_ = volumes;
    }

private:
    Program deferrred_program_;

    std::shared_ptr<OpaquePass::GBuffer> gbuffer_;
    std::shared_ptr<Volume> volumes_;
};


