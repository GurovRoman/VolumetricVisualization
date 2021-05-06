#include <gl/EnableGuard.h>
#include <ui/Parameters.h>
#include "VolumePass.h"
#include "GLcommon.h"


VolumePass::VolumePass() {
    volume_program_ = Program({"volume.vert", "volume_common.frag", "volume.frag"});

}

void VolumePass::run() {
    //// Volume Pass
    EnableGuard blend(GL_BLEND);
    EnableGuard no_depth(GL_DEPTH_TEST, false);
    glCullFace(GL_FRONT);

    output_fb_->bind();

    volume_program_.use();

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    volume_program_.setUniform1i("depth", 0);
    volume_program_.setUniform1i("vol.density", 1);

    gDepth_tex_->BindToTextureUnit(0);
    volumes_->getDensityTex3D()->BindToTextureUnit(1);



    //glUniform1i(glGetUniformLocation(volume_program_.getId(), "depth"), 0);
    volume_program_.setUniformMat4("vol.model_inv", glm::inverse(volumes_->tf.getTransformMatrix())[0][0]);
    volume_program_.setUniformMat4("VP_inv", glm::inverse(camera_->getViewProjMatrix())[0][0]);
    volume_program_.setUniformMat4("MVP", (camera_->getInfViewProjMatrix() * volumes_->tf.getTransformMatrix())[0][0]);


    volume_program_.setUniformVec3("sunDirection", (glm::inverse(glm::mat3(volumes_->tf.getRotationMatrix())) * Parameters::get().at<glm::vec3>("sunDirection"))[0]);
    volume_program_.setUniformVec3("sunColor", Parameters::get().at<glm::vec3>("sunColor")[0]);
    volume_program_.setUniform1("sunIntensity", Parameters::get().at<float>("sunIntensity"));


    volume_program_.setUniform1i("vol.samples", volumes_->samples);
    volume_program_.setUniform1i("vol.shadow_samples", volumes_->shadow_samples);
    volume_program_.setUniform1("vol.density_multiplier", volumes_->density_multiplier);
    volume_program_.setUniform1("vol.sample_point", volumes_->shadow_sample_point);
    volume_program_.setUniform1i("vol.procedural", volumes_->procedural);

    Mesh::getCube()->draw(false);


    glCullFace(GL_BACK);
}
