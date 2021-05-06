#include "Shader.h"


bool shaders_ok = true;


Shader::Shader(const std::vector<std::string>& paths, GLenum type) {
    shader_id_ = glCreateShader(type);
    paths_ = paths;

    std::vector<std::string> shader_codes;
    std::vector<const char*> shader_codes_cstr;
    for (const auto& path : paths) {
        std::ifstream file("shaders/" + path);
        shader_codes.push_back({std::istreambuf_iterator<char>(file), {}});
        shader_codes_cstr.push_back(shader_codes.back().c_str());
    }

    glShaderSource(shader_id_, shader_codes_cstr.size(), shader_codes_cstr.data() , nullptr);
    glCompileShader(shader_id_);

    checkCompilationOk();
}

void Shader::checkCompilationOk() const {
    GLint log_length;
    GLint status;

    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &status);
    glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 1){
        std::vector<char> err_msg_arr(log_length + 1);
        glGetShaderInfoLog(shader_id_, log_length, nullptr, err_msg_arr.data());
        std::string err_msg;
        for (const auto& path : paths_) {
            err_msg += path;
            err_msg += ' ';
        }
        err_msg += ": ";
        err_msg += std::string{err_msg_arr.begin(), err_msg_arr.end()};
        std::cout << err_msg << std::endl;

        if (status != GL_TRUE) {
            shaders_ok = false;
        }

    }
}

Shader::~Shader() {
    glDeleteShader(shader_id_);
}

GLuint Shader::getId() const {
    return shader_id_;
}
