#include "GLcommon.h"
#include <memory>
#include "scene/Scene.h"


using namespace glfwm;

glfwm::WindowPointer main_window;


void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "OpenGL Debug (id=" << id << "): " <<  message << std::endl;

}

int main(int argc, char** argv) {
    WindowManager::init();

    WindowManager::setHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    main_window = WindowManager::createWindow(1280, 720, std::string("Volumetric"));


    main_window->makeContextCurrent();
    gladLoadGL();
    WindowManager::setSwapInterval(0);
    main_window->doneCurrentContext();

    WindowManager::setWaitTimeout(0.);

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    main_window->bindDrawable(std::make_shared<Scene>(), 0);

    WindowGroupPointer mainGrp = WindowGroup::newGroup();
    mainGrp->attachWindow(main_window->getID());
    mainGrp->runLoopConcurrently();

    glfwSetTime(0.);

    WindowManager::mainLoop();

    // std::cout << "Average FPS was: " << frame_counter / glfwGetTime();
    
    WindowManager::terminate();
}

