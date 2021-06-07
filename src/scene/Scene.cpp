#include <ui/Parameters.h>
#include "Scene.h"
#include "gl/Shader.h"
#include <imgui/imgui.h>


Scene::Scene() {
    camera_->bindHandlers();
    camera_->tf.pos = {0, 3, 3};

    pipeline_.setMeshes(meshes_);
    pipeline_.setCamera(camera_);
    pipeline_.setVolumes(volumes_);

    volumes_->loadFromFile("csafe_heptane_302x302x302_uint8.cfg");
    volumes_->tf.pos += glm::vec3{0.f, 2.f, 0.f};

    gui_.setScene(this);

    Parameters::get().at<glm::vec3>("sunDirection") = {-1, -1, 0.1};
    Parameters::get().at<glm::vec3>("sunColor") = {1.000f, 0.879f, 0.794f};
    Parameters::get().at<float>("sunIntensity") = 1.;
    Parameters::get().at<float>("whitePoint") = 1.;

    meshes_->push_back(std::make_shared<Mesh>("1M_Cube_Chamfer_Internal.OBJ"));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}

void Scene::draw(const glfwm::WindowID id) {

    //// tick
    gui_.updateGUI();
    camera_->tick(ImGui::GetIO().DeltaTime);

    int w, h;
    glfwm::Window::getWindow(id)->getSize(w, h);
    glViewport(0, 0, w, h);
    camera_->aspect_ratio = float(w) / h;

    /// Render

    if (shaders_ok) {
        pipeline_.run();
    }

    gui_.drawGUI();
}
