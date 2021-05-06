#pragma once
#include <string>
#include "GLcommon.h"

extern bool shaders_ok;


class Shader {
public:
    explicit Shader(const std::vector<std::string>& paths, GLenum type = GL_VERTEX_SHADER);

    Shader(const Shader&) = delete;
    Shader(Shader&&) = default;
    Shader& operator =(const Shader&) = delete;
    Shader& operator =(Shader&&) = default;

    ~Shader();

    GLuint getId() const;

private:
    void checkCompilationOk() const;

private:
    GLuint shader_id_;
    std::vector<std::string> paths_;
};


