#include <gl/EnableGuard.h>
#include <ui/Parameters.h>
#include "DeferredShadingPass.h"

DeferredShadingPass::DeferredShadingPass() {
    deferrred_program_ = Program({"passthrough.vert", "volume_common.frag", "deferred_shading.frag"});
}

void DeferredShadingPass::run() {
    output_fb_->bind();

    deferrred_program_.use();

    deferrred_program_.setUniform1i("gAlbedo", 0);
    deferrred_program_.setUniform1i("gPos", 1);
    deferrred_program_.setUniform1i("gNormal", 2);

    gbuffer_->albedo->BindToTextureUnit(0);
    gbuffer_->pos->BindToTextureUnit(1);
    gbuffer_->normal->BindToTextureUnit(2);


    deferrred_program_.setUniformVec3("cameraPosition", camera_->tf.pos[0]);

    deferrred_program_.setUniformVec3("sunDirection", Parameters::get().at<glm::vec3>("sunDirection")[0]);
    deferrred_program_.setUniformVec3("sunColor", Parameters::get().at<glm::vec3>("sunColor")[0]);
    deferrred_program_.setUniform1("sunIntensity", Parameters::get().at<float>("sunIntensity"));

    deferrred_program_.setUniform1i("vol.density", 3);
    volumes_->getDensityTex3D()->BindToTextureUnit(3);

    deferrred_program_.setUniformMat4("vol.model_inv", glm::inverse(volumes_->tf.getTransformMatrix())[0][0]);
    deferrred_program_.setUniform1i("vol.samples", volumes_->samples);
    deferrred_program_.setUniform1i("vol.scene_shadow_samples", volumes_->scene_shadow_samples);
    deferrred_program_.setUniform1("vol.density_multiplier", volumes_->density_multiplier);
    deferrred_program_.setUniform1("vol.sample_point", volumes_->shadow_sample_point);
    deferrred_program_.setUniform1i("vol.procedural", volumes_->procedural);

    Mesh::getFSQuad()->draw();
}
