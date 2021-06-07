#include <ui/Parameters.h>
#include "gl/EnableGuard.h"
#include "OpaquePass.h"
#include "GLcommon.h"


OpaquePass::OpaquePass() {
    mesh_program_ = Program({"opaque_shader.vert", "opaque_shader.frag"});

    gbuffer_->albedo = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_FLOAT);
    gbuffer_->pos = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    gbuffer_->normal = std::make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA16F, GL_RGBA, GL_FLOAT);
    gbuffer_->depth = std::make_shared<Texture>(GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT);

    gbuffer_fb_.setColorAttachment(0, gbuffer_->albedo);
    gbuffer_fb_.setColorAttachment(1, gbuffer_->pos);
    gbuffer_fb_.setColorAttachment(2, gbuffer_->normal);
    gbuffer_fb_.setDepthAttachment(gbuffer_->depth);
}

void OpaquePass::run() {
    gbuffer_fb_.bind();
    gbuffer_fb_.clear();

    mesh_program_.use();

    mesh_program_.setUniformMat4("VP", (camera_->getViewProjMatrix())[0][0]);

    //// Opaque Pass
    for (const auto& mesh : *meshes_) {
        mesh_program_.setUniformVec4("solidColorSpec", glm::vec4{mesh->solid_color, mesh->specular_intensity}[0]);
        mesh->draw();
    }
}
