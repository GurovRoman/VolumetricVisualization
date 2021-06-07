#include "RenderPipeline.h"

RenderPipeline::RenderPipeline() {
    /*GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);*/

    int w, h;
    main_window->getFramebufferSize(w, h);
    final_fb_->setColorAttachment(0, std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT));
    //final_fb_->setDepthAttachment(std::make_shared<Texture>(GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT));

    volume_.setDepthTex(opaque_.getGBuffer()->depth);
    deferred_.setGBuffer(opaque_.getGBuffer());

    volume_.setOutputFramebuffer(postprocess_.getPostprocessFramebuffer());
    deferred_.setOutputFramebuffer(postprocess_.getPostprocessFramebuffer());
    postprocess_.setOutputFramebuffer(final_fb_);
}

void RenderPipeline::run() {
    glClearColor(0.192, 0.373, 0.91, 0.);
    postprocess_.getPostprocessFramebuffer()->clear();
    glClearColor(0, 0, 0, 0);
    final_fb_->clear();

    opaque_.run();
    shadowmap_.run();
    deferred_.run();
    volume_.run();
    postprocess_.run();

    int w, h;
    main_window->getFramebufferSize(w, h);

    final_fb_->bind(GL_READ_FRAMEBUFFER);
    Framebuffer::getDefaultFramebuffer()->bind(GL_DRAW_FRAMEBUFFER);
    glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Needed for OBS to capture correct framebuffer
    Framebuffer::getDefaultFramebuffer()->bind(GL_READ_FRAMEBUFFER);
}
