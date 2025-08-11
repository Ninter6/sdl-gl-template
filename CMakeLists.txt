cmake_minimum_required(VERSION 3.30)
project(sdl_gl_template)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(FILE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/")

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_executable(sdl_gl_template main.cpp glad/glad.c
        window.cpp
        window.hpp
        shader.cpp
        shader.hpp
        mesh.cpp
        mesh.hpp)
target_link_libraries(sdl_gl_template PRIVATE SDL3::SDL3)