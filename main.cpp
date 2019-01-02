#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include <vector>
#include <map>

#define WIDTH 800
#define HEIGHT 600

void processInput(GLFWwindow *);

void frameCallback(GLFWwindow *, int, int);

void mouseMoveCallback(GLFWwindow *, double, double);

void mouseScrollCallback(GLFWwindow *, double, double);

unsigned int loadImage(const char *);

void bindSpotLight(Shader &, int, mat4);

Camera camera;

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
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("%s", "load loader fail");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    //启动混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    Shader shader("../vertex.glsl", "../frag.glsl");

    unsigned int grossTexture = loadImage("../window.png");
    unsigned int marble = loadImage("../marble.jpg");

    float verticws[] = {
            // positions          // texture Coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    //加几株草
    vector<vec3> grosses{
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3(1.5f, 0.0f, 0.51f),
            glm::vec3(0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3(0.5f, 0.0f, -0.6f)
    };
    float grossesVerticws[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.5f, 0.0f, 1.0f, 0.0f
    };
    unsigned grossVao, grossVbo;
    glGenVertexArrays(1, &grossVao);

    glGenBuffers(1, &grossVbo);
    glBindBuffer(GL_ARRAY_BUFFER, grossVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grossesVerticws), grossesVerticws,
                 GL_STATIC_DRAW);

    glBindVertexArray(grossVao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticws), verticws, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.72, 0.52, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, marble);

        mat4 projection = glm::perspective(glm::radians(camera.zoom),
                                           float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", projection);
        shader.setInt("grass", 0);

        glBindVertexArray(vao);

        mat4 model;
        model = translate(model, vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = mat4();
        model = translate(model, vec3(2, 0, 0));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grossTexture);

        std::map<float, vec3> sorted;
        for (int j = 0; j < 5; ++j) {
            float distance = glm::length(camera.cameraPos - grosses[j]);
            sorted[distance] = grosses[j];
        }

        for (std::map<float, vec3>::reverse_iterator it = sorted.rbegin();
             it != sorted.rend(); ++it) {
            mat4 model;
            printf("%f\n",it->first);
            model = translate(model, it->second);
            shader.setMat4("model", model);
            glBindVertexArray(grossVao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

bool first = true;
double lastXPos, lastYPos;

void bindSpotLight(Shader &shader, int vao, mat4 projection) {
    shader.use();
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("projection", projection);
    mat4 model;
    model = translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
    model = scale(model, vec3(0.2));
    shader.setMat4("model", model);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void mouseMoveCallback(GLFWwindow *window, double xPos, double yPos) {
    if (first) {
        first = false;
        lastXPos = xPos;
        lastYPos = yPos;
    }
    double xOffset = xPos - lastXPos;
    double yOffset = lastYPos - yPos;
    lastXPos = xPos;
    lastYPos = yPos;
    camera.moveMouse(xOffset, yOffset);
}

void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    camera.scrollMouse(yOffset);
}

double lastTime;

void processInput(GLFWwindow *window) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        camera.moveKey(CameraMovement::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        camera.moveKey(CameraMovement::DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        camera.moveKey(CameraMovement::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        camera.moveKey(CameraMovement::RIGHT, deltaTime);
    }
}

void frameCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int loadImage(const char *path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum gLenum;
        if (nrChannels == 1) {
            gLenum = GL_RED;
        } else if (nrChannels == 3) {
            gLenum = GL_RGB;
        } else if (nrChannels == 4) {
            gLenum = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, gLenum, width, height, 0, gLenum, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    return texture;
}