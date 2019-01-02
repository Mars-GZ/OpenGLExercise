cmake_minimum_required(VERSION 3.12)
project(light)

set(CMAKE_CXX_STANDARD 11)

add_executable(light main.cpp ../stb_image.cpp ../glad.c ../stb_image.h
        ../shader.h ../camera.h)

target_link_libraries(light glfw GL)