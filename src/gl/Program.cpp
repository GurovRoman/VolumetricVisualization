#include "Program.h"
#include "Shader.h"


Program::Program(std::initializer_list<std::string> paths) {
    for (const auto& path : paths) {
        if (path.ends_with(".vert")) {
            vertex_paths_.push_back(path);
        } else if (path.ends_with(".frag")) {
            fragment_paths_.push_back(path);
        }
    }

    reload();
}

Program::~Program() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
}

void Program::reload() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
    }
    program_id_ = glCreateProgram();

    Shader vs(vertex_paths_, GL_VERTEX_SHADER);
    glAttachShader(program_id_, vs.getId());

    Shader fs(fragment_paths_, GL_FRAGMENT_SHADER);
    glAttachShader(program_id_, fs.getId());

    glLinkProgram(program_id_);

    GLsizei log_length = 200;
    char log[200];
    glGetProgramInfoLog(program_id_, 200, &log_length, log);
    if (log_length > 0)
        std::cout << log << '\n';
}

void Program::reloadAllActive() {
    shaders_ok = true;
    for (auto& prog : decltype(instance_keeper)::getInstances()) {
        prog->reload();
    }
}

GLuint Program::getId() const {
    return program_id_;
}

GLint Program::getUniformLocation(const char* name) const {
    return glGetUniformLocation(program_id_, name);
}

void Program::setUniformMat4(const char* name, const GLfloat& data, GLsizei count, GLboolean transpose) {
    glUniformMatrix4fv(getUniformLocation(name), count, transpose, &data);
}

void Program::setUniformVec3(const char* name, const GLfloat& data, GLsizei count) {
    glUniform3fv(getUniformLocation(name), count, &data);
}

void Program::setUniformVec4(const char* name, const GLfloat& data, GLsizei count) {
    glUniform4fv(getUniformLocation(name), count, &data);
}

void Program::setUniform1(const char* name, const GLfloat& data, GLsizei count) {
    glUniform1fv(getUniformLocation(name), count, &data);
}

void Program::setUniform1i(const char* name, const GLint& data, GLsizei count) {
    glUniform1iv(getUniformLocation(name), count, &data);
}

void Program::use() {
    glUseProgram(program_id_);
}
