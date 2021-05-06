target_compile_options(VolumetricRayMarching PRIVATE -O0)

target_sources(
        VolumetricRayMarching PRIVATE
        main.cpp
        scene/Camera.cpp
        scene/Scene.cpp
        scene/Volume.cpp
        scene/Volume.h
        scene/Mesh.cpp
        scene/Mesh.h
        scene/Transform.cpp
        scene/Transform.h
        gl/Shader.cpp
        gl/Shader.h
        gl/Program.cpp
        gl/Program.h
        gl/Texture.cpp
        gl/Texture.h
        gl/Framebuffer.cpp
        gl/Framebuffer.h
        gl/EnableGuard.cpp
        gl/EnableGuard.h
        pipeline/RenderPipeline.cpp
        pipeline/RenderPipeline.h
        pipeline/RenderPass.h
        pipeline/passes/OpaquePass.cpp
        pipeline/passes/OpaquePass.h
        pipeline/passes/VolumePass.cpp
        pipeline/passes/PostprocessPass.cpp
        pipeline/passes/DeferredShadingPass.cpp
        pipeline/passes/DeferredShadingPass.h
        ui/Parameters.cpp
        ui/Parameters.h
        ui/GUI.cpp
        ui/GUI.h
        util/imGuIZMO.quat/imGuIZMOquat.cpp)


target_include_directories(VolumetricRayMarching PRIVATE .)


find_package(OpenGL REQUIRED)

target_link_libraries(VolumetricRayMarching glfwm)
target_link_libraries(VolumetricRayMarching OpenGL::GL)
target_link_libraries(VolumetricRayMarching glm)

