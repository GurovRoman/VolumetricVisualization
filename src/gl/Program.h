#pragma once
#include <string>
#include <util/GlobalInstanceKeeper.h>
#include "GLcommon.h"


class Program {
public:
    Program() = default;

    Program(const Program&) = delete;
    Program(Program&& other) noexcept { operator=(std::move(other)); };
    Program& operator =(const Program&) = delete;
    Program& operator =(Program&& other) noexcept {
        if (program_id_ != 0) {
            glDeleteProgram(program_id_);
        }
        fragment_paths_ = std::move(other.fragment_paths_);
        vertex_paths_ = std::move(other.vertex_paths_);
        program_id_ = other.program_id_;
        other.program_id_ = 0;

        return *this;
    }

    Program(std::initializer_list<std::string> paths);

    ~Program();

    void reload();

    static void reloadAllActive();

    void use();

    GLuint getId() const;
    GLint getUniformLocation(const char* name) const;
    void setUniformMat4(const char* name, const GLfloat& data, GLsizei count = 1, GLboolean transpose = GL_FALSE);
    void setUniformVec3(const char* name, const GLfloat& data, GLsizei count = 1);
    void setUniformVec4(const char* name, const GLfloat& data, GLsizei count = 1);
    void setUniform1i(const char* name, const GLint& data, GLsizei count = 1);
    void setUniform1(const char* name, const GLfloat& data, GLsizei count = 1);

private:
    std::vector<std::string> fragment_paths_;
    std::vector<std::string> vertex_paths_;
    GLuint program_id_ {0};

    GlobalInstanceKeeper<Program*> instance_keeper {this};
};


