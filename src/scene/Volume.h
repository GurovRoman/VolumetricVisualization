#pragma once
#include <vector>
#include <string>
#include <gl/Texture.h>
#include "glm/vec3.hpp"
#include "Actor.h"


class Volume : public Actor {
private:
    std::vector<GLfloat> volume_data_;
    glm::uvec3 size_{};
    TexturePtr density_tex3d_;

public:
    GLint samples {20};
    GLint shadow_samples {10};
    GLint scene_shadow_samples {30};
    GLfloat density_multiplier {1.f};
    GLfloat shadow_sample_point {.25f};
    GLboolean procedural {false};

public:
    GLfloat operator ()(unsigned x, unsigned y, unsigned z);

    void loadFromFile(const std::string& path);

    inline const TexturePtr& getDensityTex3D() const {
        return density_tex3d_;
    }

    static const std::vector<std::string>& getVolumeFileList();
};

