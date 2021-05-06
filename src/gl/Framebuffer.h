#pragma once
#include <memory>
#include <array>
#include "Texture.h"


class Framebuffer {
public:
    using TargetPtr = std::shared_ptr<Texture>;

public:
    Framebuffer();

    void setColorAttachment(size_t i, const TargetPtr& tex);
    void setDepthAttachment(const TargetPtr& tex);

    inline TargetPtr getColorAttachment(size_t i) const {
        return color_attachments_[i];
    }

    inline TargetPtr getDepthAttachment() const {
        return depth_attachment_;
    }

    void bind(GLenum type = GL_FRAMEBUFFER);
    inline GLuint getId() const {
        return framebuffer_id_;
    }

    void resize(size_t w, size_t h);

    void clear();

    static const std::shared_ptr<Framebuffer>& getDefaultFramebuffer();

private:
    explicit Framebuffer(GLuint framebuffer_id) : framebuffer_id_(framebuffer_id) {}
    void reattachDrawbuffers();

public:
    static const size_t max_color_attachments = 8;

private:
    GLuint framebuffer_id_;
    std::array<TargetPtr, max_color_attachments> color_attachments_;
    TargetPtr depth_attachment_;
};

