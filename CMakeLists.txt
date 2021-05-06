cmake_minimum_required(VERSION 3.17)
project(VolumetricRayMarching)

set(CMAKE_CXX_STANDARD 20)

set(glfw3_DIR ${CMAKE_CURRENT_LIST_DIR}/glfw)
add_subdirectory(glfwm)

add_subdirectory(glm)


add_executable(
        VolumetricRayMarching
        glad/src/glad.c)

add_subdirectory(src)

target_include_directories(VolumetricRayMarching PRIVATE glad/include)

add_custom_target(
        resource
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_BINARY_DIR}/models
        #COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/volumes
        #${CMAKE_CURRENT_BINARY_DIR}/volumes
)

add_dependencies(VolumetricRayMarching resource)



target_include_directories(VolumetricRayMarching PRIVATE imgui)
target_sources(
        VolumetricRayMarching PRIVATE
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp)

add_compile_definitions(IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS)

add_compile_definitions(VGIZMO_USES_GLM)

add_compile_definitions(FAST_OBJ_IMPLEMENTATION)