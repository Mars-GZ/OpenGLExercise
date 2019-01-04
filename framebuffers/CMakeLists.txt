cmake_minimum_required(VERSION 3.12)
project(framebuffer)

set(CMAKE_CXX_STANDARD 11)

add_executable(framebuffer main.cpp ../stb_image.cpp ../glad.c ../stb_image.h
        ../shader.h ../camera.h)

target_link_libraries(framebuffer glfw GL)