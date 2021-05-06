#include <imgui.h>
#include <imgui_internal.h>
#include "GUI.h"
#include <GLcommon.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <util/imGuIZMO.quat/imGuIZMOquat.h>
#include "Parameters.h"
#include <scene/Scene.h>


GUI::GUI() {
    ImGui::CreateContext();

    main_window->makeContextCurrent();
    ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
    main_window->doneCurrentContext();

    ImGui_ImplOpenGL3_Init(nullptr);
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::updateGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    drawFrametime();
    drawMain();

    ImGui::Render();
}

void GUI::drawGUI() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::drawMain() {
    ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Actions")) {
            if (ImGui::MenuItem("Reload Shaders")) {
                Program::reloadAllActive();
            }
            static bool vsync_enabled = false;
            if (ImGui::MenuItem("Enable VSync", nullptr, &vsync_enabled)) {
                main_window->makeContextCurrent();
                glfwm::WindowManager::setSwapInterval(vsync_enabled);
                main_window->doneCurrentContext();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create")) {
            for (const auto& mesh : Mesh::getMeshFileList()) {
                if (ImGui::MenuItem((mesh + "##meshentry").c_str())) {
                    scene_->getMeshes()->emplace_back(mesh);
                }
            }
            for (const auto& volume : Volume::getVolumeFileList()) {
                if (ImGui::MenuItem((volume + "##volumeentry").c_str())) {
                    scene_->getVolumes()->loadFromFile(volume);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::gizmo3D("Sun Direction", Parameters::get().at<glm::vec3>("sunDirection"));

    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::PushItemWidth(130);
    ImGui::ColorEdit3("Sun Color", &Parameters::get().at<glm::vec3>("sunColor")[0]);
    ImGui::DragFloat("Sun Intensity", &Parameters::get().at<float>("sunIntensity"), 0.005f, 0.f, FLT_MAX);
    ImGui::DragFloat("White Point", &Parameters::get().at<float>("whitePoint"), 0.005f, 0.005f, FLT_MAX);
    ImGui::PopItemWidth();
    ImGui::EndGroup();

    ImGui::Separator();

    drawActorListWidget();

    ImGui::End();
}

void GUI::drawProperties(const std::string& name, Actor* actor, bool& open) {
    ImGui::Begin(("Property browser: " + name + "###propbrowse").c_str(), &open, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PushItemWidth(130);

    ImGui::DragFloat3("Position", &actor->tf.pos[0], 0.05f);
    ImGui::DragFloat3("Angle", &actor->tf.angle[0], 1);
    actor->tf.angle = glm::mod(actor->tf.angle, 360.f);
    ImGui::DragFloat3("Scale", &actor->tf.scale[0], 0.05f, 0.05f, FLT_MAX);
    {
        auto& scale_ref = actor->tf.scale;
        float proportional_scale_old = (scale_ref.x + scale_ref.y + scale_ref.z) / 3;
        float proportional_scale_new = proportional_scale_old;
        if (ImGui::DragFloat("Scale", &proportional_scale_new, 0.05f, 0.05f, FLT_MAX)) {
            scale_ref *= proportional_scale_new / proportional_scale_old;
        }
    }

    ImGui::Separator();

    if (auto* volume = dynamic_cast<Volume*>(actor)) {
        ImGui::DragInt("Samples##volprop", &volume->samples, 1, 1, 100);
        ImGui::DragInt("Shadow samples", &volume->shadow_samples, 1, 0, 100);
        ImGui::DragInt("Scene shdw smpls", &volume->scene_shadow_samples, 1, 0, 100);
        ImGui::DragFloat("Density multiplier", &volume->density_multiplier, 0.5, 0.5, 100.);
        ImGui::DragFloat("Shadow sample bias", &volume->shadow_sample_point, 0.05, 0., 1.);
        int procedural = volume->procedural;
        ImGui::RadioButton("From File", &procedural, false); ImGui::SameLine();
        ImGui::RadioButton("From Shader Function", &procedural, true);
        volume->procedural = procedural;
    }

    if (auto* mesh = dynamic_cast<Mesh*>(actor)) {
        ImGui::ColorEdit3("Color##meshprop", &mesh->solid_color[0]);
        ImGui::DragFloat("Specular Intensity##meshprop", &mesh->specular_intensity, 0.01f, 0, 2);
    }

    ImGui::End();
}

void GUI::drawActorListWidget() {
    static Actor* selection = nullptr;
    static std::string selection_name;
    if (ImGui::CollapsingHeader("Object List")) {
        if (ImGui::BeginListBox("##actorlist")) {
            for (auto& mesh : *scene_->getMeshes()) {
                std::ostringstream name("Mesh ", std::ios_base::app);
                name << &mesh;
                if (ImGui::Selectable(name.str().c_str(), selection == &mesh)) {
                    selection = &mesh;
                    selection_name = name.str();
                }
            }
            if (ImGui::Selectable("Volume", selection == &*scene_->getVolumes())) {
                selection = &*scene_->getVolumes();
                selection_name = "Volume";
            }
            ImGui::EndListBox();
        }
    }
    if (selection) {
        bool open = true;
        drawProperties(selection_name, selection, open);
        if (!open) {
            selection = nullptr;
        }
    }
}

void GUI::drawFrametime() {
    auto ctx = ImGui::GetCurrentContext();
    auto io = ImGui::GetIO();
    static std::list<std::pair<double, float>> frames;
    static double running_average;
    frames.emplace_back(ImGui::GetTime(), io.DeltaTime);
    running_average += io.DeltaTime;
    while (frames.size() > 1 && frames.front().first < ImGui::GetTime() + 2.) {
        running_average -= frames.front().second;
        frames.pop_front();
    }

    double average_frametime = running_average / frames.size();

    auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize;
    ImGui::Begin("Frametime", nullptr, flags);

    ImGui::PlotLines("##frametimes", ctx->FramerateSecPerFrame, IM_ARRAYSIZE(ctx->FramerateSecPerFrame), ctx->FramerateSecPerFrameIdx,
                     nullptr, 0.0f, 0.05f, ImVec2(0, 80.0f));
    ImGui::Text("%.3f ms", average_frametime * 1000.f);

    ImGui::End();
}
