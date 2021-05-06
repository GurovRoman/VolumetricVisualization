#include <GLcommon.h>
#include <gl/EnableGuard.h>
#include <ui/Parameters.h>
#include "PostprocessPass.h"


PostprocessPass::PostprocessPass() {
    int w, h;
    main_window->getFramebufferSize(w, h);
    input_fb_->setColorAttachment(0, std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT));
    input_fb_->setDepthAttachment(std::make_shared<Texture>(GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT));
}

void PostprocessPass::run() {
    EnableGuard srgb(GL_FRAMEBUFFER_SRGB);
    EnableGuard no_depth(GL_DEPTH_TEST, false);

    output_fb_->bind();

    postprocess_program_.use();

    input_fb_->getColorAttachment(0)->BindToTextureUnit(0);

    postprocess_program_.setUniform1("whitePoint", Parameters::get().at<float>("whitePoint"));

    Mesh::getFSQuad()->draw();
}
