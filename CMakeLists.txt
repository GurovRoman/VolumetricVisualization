cmake_minimum_required(VERSION 3.17)
project(VolumetricRayMarching)

set(CMAKE_CXX_STANDARD 20)


set(glfw3_DIR ${CMAKE_CURRENT_LIST_DIR}/glfw)

add_subdirectory(glfwm ${${PROJECT_NAME}_BINARY_DIR}/glfwm)

add_compile_options(-O3)

add_executable(VolumetricRayMarching
        main.cpp
        math.cpp
        Camera.cpp
        Scene.cpp
        glad/src/glad.c Volume.cpp Volume.h)


target_link_libraries(VolumetricRayMarching glfwm)

find_package(OpenGL REQUIRED)
target_link_libraries(VolumetricRayMarching OpenGL::GL)

target_include_directories(VolumetricRayMarching PUBLIC glad/include)
