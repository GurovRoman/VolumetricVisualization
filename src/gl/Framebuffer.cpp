#include "Framebuffer.h"
#include <algorithm>


Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &framebuffer_id_);
    this->bind();
    glDrawBuffer(GL_NONE);
}

void Framebuffer::setColorAttachment(size_t i, const Framebuffer::TargetPtr& tex) {
    color_attachments_[i] = tex;
    this->bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex ? tex->getId() : 0, 0);
    reattachDrawbuffers();
}

void Framebuffer::setDepthAttachment(const Framebuffer::TargetPtr& tex) {
    depth_attachment_ = tex;
    this->bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex ? tex->getId() : 0, 0);
}

void Framebuffer::bind(GLenum type) {
    glBindFramebuffer(type, framebuffer_id_);
}

void Framebuffer::resize(size_t w, size_t h) {
    for (auto& target : color_attachments_) {

    }
    depth_attachment_;
}

void Framebuffer::clear() {
    this->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::reattachDrawbuffers() {
    std::vector<GLenum> bufs;
    bufs.reserve(max_color_attachments);
    for (size_t i = 0; i < color_attachments_.size(); ++i) {
        bufs.push_back(color_attachments_[i] ? GL_COLOR_ATTACHMENT0 + i : GL_NONE);
    }
    glDrawBuffers(bufs.size(), bufs.data());
}

const std::shared_ptr<Framebuffer>& Framebuffer::getDefaultFramebuffer() {
    static std::shared_ptr<Framebuffer> default_fb = std::make_shared<Framebuffer>(std::move(Framebuffer(0)));
    return default_fb;
}
