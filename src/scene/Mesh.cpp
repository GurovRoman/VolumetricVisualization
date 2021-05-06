#include "Mesh.h"
#include "Actor.h"
#include <util/fast_obj.h>
#include <filesystem>


Mesh::Mesh(const std::string& path) {

    auto mesh = fast_obj_read(("models/" + path).c_str());

    if (mesh == nullptr) {
        std::cout << "OBJ parse failed: " << path << '\n';
        return;
    }

    for (size_t i = 0; i < mesh->position_count; ++i) {
        vertices_.emplace_back(mesh->positions[3 * i], mesh->positions[3 * i + 1], mesh->positions[3 * i + 2]);
        normals_.emplace_back(mesh->normals[3 * i], mesh->normals[3 * i + 1], mesh->normals[3 * i + 2]);
        uvs_.emplace_back(mesh->texcoords[2 * i], mesh->texcoords[2 * i + 1]);
        vertices_.back() /= 50;
    }
    for (size_t i = 0; i < mesh->face_count; ++i) {
        triangles_.emplace_back(mesh->indices[3 * i].p, mesh->indices[3 * i + 1].p, mesh->indices[3 * i + 2].p);
    }


    uploadBuffers();

    fast_obj_destroy(mesh);
}

void Mesh::draw(bool uploadModelMatrix) const {
    if (uploadModelMatrix) {
        GLint pr_id;
        glGetIntegerv(GL_CURRENT_PROGRAM, &pr_id);
        auto m_id = glGetUniformLocation(pr_id, "model");
        auto n_id = glGetUniformLocation(pr_id, "normalMatrix");

        glUniformMatrix4fv(m_id, 1, GL_FALSE, &(tf.getTransformMatrix())[0][0]);
        glUniformMatrix3fv(n_id, 1, GL_TRUE, &(glm::inverse(glm::mat3(tf.getTransformMatrix())))[0][0]);
    }

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id_);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_id_);
    glDrawElements(GL_TRIANGLES, triangles_.size() * 3, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh::uploadBuffers() {
    glGenBuffers(1, &vertex_buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(decltype(vertices_[0])), vertices_.data(), GL_STATIC_DRAW);

    normals_.resize(vertices_.size());
    glGenBuffers(1, &normal_buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, normals_.size() * sizeof(decltype(normals_[0])), normals_.data(), GL_STATIC_DRAW);

    uvs_.resize(vertices_.size());
    glGenBuffers(1, &uv_buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, uvs_.size() * sizeof(decltype(uvs_[0])), uvs_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &triangle_buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles_.size() * sizeof(decltype(triangles_[0])), triangles_.data(), GL_STATIC_DRAW);
}

const std::shared_ptr<Mesh>& Mesh::getCube() {
    static std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(std::move(Mesh()));
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        float size = 1;
        float l = size / 2;
        for (int i = 0; i < 8; i++) {
            cube->vertices_.emplace_back(
                    (i & 4) != 0 ? l : -l,
                    (i & 2) != 0 ? l : -l,
                    (i & 1) != 0 ? l : -l);
            auto v = cube->vertices_.back();
        }

        auto AddVert = [](int one, int two, int three){
            cube->triangles_.emplace_back(one, two, three);
        };

        for (int i = 0; i < 3; i++) {
            int v1 = 1 << i;
            int v2 = v1 == 4 ? 1 : v1 << 1;
            AddVert(0, v1, v2);
            AddVert(v1 + v2, v2, v1);
            AddVert(7, 7 - v2, 7 - v1);
            AddVert(7 - (v1 + v2), 7 - v1, 7 - v2);
        }

        cube->uploadBuffers();
    }
    return cube;
}

const std::shared_ptr<Mesh>& Mesh::getFSQuad() {
    static std::shared_ptr<Mesh> quad = std::make_shared<Mesh>(std::move(Mesh()));
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        quad->vertices_ = {
                {-1.0f, -1.0f, 0.0f},
                {1.0f, -1.0f, 0.0f},
                {1.0f, 1.0f, 0.0f},
                {-1.0f, 1.0f, 0.0f}
        };

        quad->triangles_ = {
                {0, 1, 3},
                {2, 3, 1}
        };

        quad->uploadBuffers();
    }
    return quad;
}

const std::vector<std::string>& Mesh::getMeshFileList() {
    static std::vector<std::string> list;
    static bool ready = false;
    if (!ready) {
        list.clear();
        for (const auto& entry : std::filesystem::directory_iterator("models")) {
            const auto& path = entry.path().filename().string();
            if (!entry.is_directory() && path.ends_with(".OBJ")) {
                list.push_back(path);
            }
        }
    }
    return list;
}

