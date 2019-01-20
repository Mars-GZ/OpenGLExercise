cmake_minimum_required(VERSION 3.12)
project(highGLSL)

set(CMAKE_CXX_STANDARD 11)

add_executable(highGLSL main.cpp stb_image.cpp glad.c ../stb_image.h
        ../shader.h ../camera.h)

target_link_libraries(highGLSL glfw GL)