#pragma once
#include "glm/vec3.hpp"
#include "Transform.h"
#include "GLcommon.h"
#include "Actor.h"
#include <vector>

class Mesh : public Actor {
public:
    explicit Mesh(const std::string& path);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = default;
    Mesh& operator =(const Mesh&) = delete;
    Mesh& operator =(Mesh&&) = default;

    void draw(bool uploadModelMatrix=true) const;

    void uploadBuffers();

    static const std::vector<std::string>& getMeshFileList();
    static const std::shared_ptr<Mesh>& getCube();
    static const std::shared_ptr<Mesh>& getFSQuad();

private:
    Mesh() = default;

public:
    glm::vec3 solid_color {0.458, 0.458, 0.458};
    float specular_intensity {0.2};


private:

    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;
    std::vector<glm::vec2> uvs_;
    std::vector<glm::uvec3> triangles_;
    GLuint vertex_buffer_id_ {0};
    GLuint normal_buffer_id_ {0};
    GLuint uv_buffer_id_ {0};
    GLuint triangle_buffer_id_ {0};
};


