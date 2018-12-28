#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#define WIDTH 800
#define HEIGHT 600

void processInput(GLFWwindow *);

void frameCallback(GLFWwindow *, int, int);

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPEN_FORWARD_COMPAT,GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "High OpenGL", NULL, NULL);
    if (window == nullptr) {
        printf("%s", "fail to create GLFW Window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("%s", "load loader fail");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);



    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.72, 0.52, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

void frameCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}