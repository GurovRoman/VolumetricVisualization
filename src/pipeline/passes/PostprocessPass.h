#pragma once
#include <pipeline/RenderPass.h>


class PostprocessPass : public RenderPass {
public:
    PostprocessPass();
    void run() override;

    inline const auto& getPostprocessFramebuffer() {
        return input_fb_;
    }

private:
    Program postprocess_program_ {"passthrough.vert", "postprocess.frag"};
    std::shared_ptr<Framebuffer> input_fb_ {std::make_shared<Framebuffer>()};
};


