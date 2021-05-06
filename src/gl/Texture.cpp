#include "Texture.h"

Texture::Texture(GLenum target, GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height, const void *pixels)
  : target_(target), internal_format_(internal_format), format_(format), type_(type)
{
    glGenTextures(1, &texture_id_);
    glBindTexture(target, texture_id_);
    if (width == -1) {
        glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int w, h;
        main_window->getFramebufferSize(w, h);
        glTexImage2D(target_, 0, internal_format_, w, h, 0, format_, type_, nullptr);
    } else {
        glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(target_, 0, internal_format_, width, height, 0, format_, type_, pixels);
    }
}


Texture::Texture(GLenum target, GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height,
                 GLsizei depth, const void* pixels)
  : target_(target), internal_format_(internal_format), format_(format), type_(type)
{
    glGenTextures(1, &texture_id_);
    glBindTexture(target_, texture_id_);
    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(target_, 0, internal_format_, width, height, depth, 0, format_, type_, pixels);
    glTexParameteri(target_, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void Texture::BindToTextureUnit(size_t i) const {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(target_, texture_id_);
}
