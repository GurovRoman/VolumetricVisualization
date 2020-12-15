add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libglfw3.a)

target_include_directories(glfw INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
