cmake_minimum_required(VERSION 3.12)
project(sky)

set(CMAKE_CXX_STANDARD 11)

add_executable(sky main.cpp ../stb_image.cpp ../glad.c ../stb_image.h
        ../shader.h ../camera.h)

target_link_libraries(sky glfw GL)