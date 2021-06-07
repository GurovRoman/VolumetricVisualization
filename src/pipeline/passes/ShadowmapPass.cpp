#include <ui/Parameters.h>
#include "ShadowmapPass.h"
#include <glm/glm.hpp>


ShadowmapPass::ShadowmapPass() {
    mesh_program_ = Program({"opaque_shader.vert", "opaque_shader.frag"});

    shadowmap_ = std::make_shared<Texture>(GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, shadowmap_resolution, shadowmap_resolution);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderDepth[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderDepth);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

    draw_buffer_.setDepthAttachment(shadowmap_);

    Parameters::get().at<TexturePtr>("sunShadowmap") = shadowmap_;
}

void ShadowmapPass::run() {
    draw_buffer_.bind();
    draw_buffer_.clear();

    glCullFace(GL_FRONT);

    GLint old_viewport[4];
    glGetIntegerv(GL_VIEWPORT, old_viewport);
    glViewport(0, 0, shadowmap_resolution, shadowmap_resolution);

    Camera sun_camera;
    sun_camera.perspective = false;
    auto dir = glm::normalize(Parameters::get().at<glm::vec3>("sunDirection"));

    sun_camera.tf.angle.x = glm::degrees(std::asin(dir.y));
    sun_camera.tf.angle.y = -glm::degrees(std::atan2(dir.z, dir.x)) - 90;

    Parameters::get().at<glm::mat4>("sunShadowVPMatrix") = sun_camera.getViewProjMatrix();

    mesh_program_.use();

    mesh_program_.setUniformMat4("VP", (sun_camera.getViewProjMatrix())[0][0]);

    //// Depth Pass
    for (const auto& mesh : *meshes_) {
        mesh_program_.setUniformVec4("solidColorSpec", glm::vec4{mesh->solid_color, mesh->specular_intensity}[0]);
        mesh->draw();
    }

    glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
    glCullFace(GL_BACK);
}
