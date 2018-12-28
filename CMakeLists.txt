cmake_minimum_required(VERSION 3.12)
project(highOpenGL)

set(CMAKE_CXX_STANDARD 11)

add_executable(highOpenGL main.cpp stb_image.cpp glad.c stb_image.h shader.h camera.h)
target_link_libraries(highOpenGL glfw GL)