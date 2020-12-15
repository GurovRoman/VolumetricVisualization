#include "GLcommon.h"
#include <memory>
#include "Scene.h"


using namespace glfwm;


int main(int argc, char** argv) {
    WindowManager::init();

    WindowPointer mainWin = WindowManager::createWindow(800, 600, std::string("Volumetric"));


    mainWin->makeContextCurrent();
    gladLoadGL();
    WindowManager::setSwapInterval(1);
    mainWin->doneCurrentContext();

    WindowManager::setWaitTimeout(0.);


    mainWin->bindDrawable(std::make_shared<Scene>(), 0);

    WindowGroupPointer mainGrp = WindowGroup::newGroup();
    mainGrp->attachWindow(mainWin->getID());
    mainGrp->runLoopConcurrently();

    glfwSetTime(0.);

    WindowManager::mainLoop();

    std::cout << "Average FPS was: " << frame_counter / glfwGetTime();
    
    WindowManager::terminate();
}

