#pragma once
#include <GLcommon.h>



class Texture {
public:
    Texture(GLenum target, GLint internal_format, GLenum format, GLenum type, GLsizei width = -1, GLsizei height = -1, const void *pixels = nullptr);
    Texture(GLenum target, GLint internal_format, GLenum format, GLenum type, GLsizei width, GLsizei height, GLsizei depth, const void *pixels = nullptr);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = default;
    Texture& operator =(const Texture&) = delete;
    Texture& operator =(Texture&&) = default;

    inline GLuint getId() const {
        return texture_id_;
    }

    void BindToTextureUnit(size_t i) const;

private:
    GLuint texture_id_;

    GLenum target_;
    GLint internal_format_;
    GLenum format_;
    GLenum type_;
};


using TexturePtr = std::shared_ptr<Texture>;
