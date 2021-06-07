#include "Camera.h"
#include <cmath>
#include <GLcommon.h>
#include <imgui/imgui.h>
#include "glm/gtc/matrix_transform.hpp"

glm::vec3 Camera::GetCameraPixelPosition(int x, int y, int pixel_width, int pixel_height) {
    auto height = std::tan(M_PI * fov / 360) * 2;
    auto width = height * (double(pixel_width) / pixel_height);

    return tf.pos + glm::vec3(1, -width / 2, -height / 2)
           + glm::vec3(0, width * x / pixel_width, height * y / pixel_height);
}

glm::mat4x4 Camera::getViewProjMatrix() const {
    glm::mat4 proj_mat;
    if (perspective) {
        proj_mat = glm::perspective(glm::radians(fov), aspect_ratio, 0.2f, 50.f);;
    } else {
        proj_mat = glm::ortho<float>(-5, 5, -5, 5, -5, 5);
    }
    return proj_mat * glm::inverse(tf.getTransformMatrix());
}

glm::mat4x4 Camera::getInfViewProjMatrix() const {
    auto proj_mat = glm::infinitePerspective(glm::radians(fov), aspect_ratio, 0.2f);
    return proj_mat * glm::inverse(tf.getTransformMatrix());
}


class CameraInputHandler : public glfwm::EventHandler {
public:
    explicit CameraInputHandler(std::shared_ptr<Camera> camera) : camera_(std::move(camera)) {}

    glfwm::EventBaseType getHandledEventTypes() const override {
        return glfwm::EventType::MOUSE_BUTTON | glfwm::EventType::CURSOR_POSITION;
    }

    bool handle(const glfwm::EventPointer &e) override {
        auto io = ImGui::GetIO();
        if (e->getEventType() == glfwm::EventType::MOUSE_BUTTON && !io.WantCaptureMouse) {
            auto mb = std::dynamic_pointer_cast<glfwm::EventMouseButton>(e);
            if (mb->getMouseButton() != glfwm::MouseButtonType::MOUSE_BUTTON_RIGHT) {
                return false;
            }
            if (mb->getAction() == glfwm::ActionType::PRESS) {
                controlling_camera_ = true;
            } else if (mb->getAction() == glfwm::ActionType::RELEASE) {
                controlling_camera_ = false;
            } else {
                return false;
            }
            return true;
        }
        if (e->getEventType() == glfwm::EventType::CURSOR_POSITION) {
            auto cp = std::dynamic_pointer_cast<glfwm::EventCursorPosition>(e);

            // Update old coordinates even if not controlling
            float deltax = cp->getX() - prevx;
            float deltay = cp->getY() - prevy;

            prevx = cp->getX();
            prevy = cp->getY();

            if (!controlling_camera_) {
                return false;
            }

            camera_->tf.angle.x = glm::clamp(camera_->tf.angle.x - deltay, -90.f, 90.f);
            camera_->tf.angle.y = glm::mod(camera_->tf.angle.y - deltax, 360.f);

            return true;
        }

        return false;
    }

    void tick(float timeDelta) {
        if (!controlling_camera_)
            return;

        auto io = ImGui::GetIO();

        if (io.WantCaptureKeyboard)
            return;

        using glfwm::KeyType, glfwm::ActionType, glm::vec3;

        auto keyPressed = [] (KeyType key) {
            return main_window->getKey(key) == ActionType::PRESS;
        };

        vec3 translation{};

        if (keyPressed(KeyType::KEY_W)) translation += vec3{0.f, 0.f, -1.f};
        if (keyPressed(KeyType::KEY_A)) translation += vec3{-1.f, 0.f, 0.f};
        if (keyPressed(KeyType::KEY_S)) translation += vec3{0.f, 0.f, 1.f};
        if (keyPressed(KeyType::KEY_D)) translation += vec3{1.f, 0.f, 0.f};

        translation = camera_->tf.getRotationMatrix() * glm::vec4(translation, 1);

        if (keyPressed(KeyType::KEY_SPACE)) translation += vec3{0.f, 1.f, 0.f};
        if (keyPressed(KeyType::KEY_LEFT_CTRL)) translation += vec3{0.f, -1.f, 0.f};

        if (glm::length(translation) > 1e-6)
            camera_->tf.pos += glm::normalize(translation) * camera_->speed * timeDelta;

    }

private:
    bool controlling_camera_ {false};
    std::shared_ptr<Camera> camera_;
    double prevx {0}, prevy {0};
};

void Camera::bindHandlers() {
    auto handler = std::make_shared<CameraInputHandler>(shared_from_this());
    main_window->bindEventHandler(handler, 0);
    input_handler = std::move(handler);
}

void Camera::tick(float timeDelta) {
    if (input_handler.has_value()) {
        std::any_cast<std::shared_ptr<CameraInputHandler>&>(input_handler)->tick(timeDelta);
    }
}


