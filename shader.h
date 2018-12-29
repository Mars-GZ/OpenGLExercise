//
// Created by mesmers on 18-12-28.
//

#ifndef HIGHOPENGL_SHADER_H
#define HIGHOPENGL_SHADER_H

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
    unsigned int id;

    Shader(const GLchar *vertexPath, const GLchar *fragPath) {
        ifstream vertexFile, fragFile;
        stringstream vertexStream, fragStream;

        vertexFile.open(vertexPath);
        fragFile.open(fragPath);
        vertexStream << vertexFile.rdbuf();
        fragStream << fragFile.rdbuf();
        vertexFile.close();
        fragFile.close();

        string svertexCode;
        string sfragCode;
        svertexCode = vertexStream.str();
        sfragCode = fragStream.str();
        const char *vertexCode = svertexCode.c_str();
        const char *fragCode = sfragCode.c_str();

        unsigned int vertex, frag;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCode, NULL);
        glCompileShader(vertex);
        checkError(vertex, "Shader");

        frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fragCode, NULL);
        glCompileShader(frag);
        checkError(frag, "Shader");

        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, frag);
        glLinkProgram(id);
        checkError(id, "Program");

        glDeleteShader(vertex);
        glDeleteShader(frag);
    }

    void use() {
        glUseProgram(id);
    }

    void setBool(const char *name, bool value) const {
        glUniform1i(getLocation(name), value);
    }

    void setInt(const char *name, int value) const {
        glUniform1i(getLocation(name), value);
    }

    void setFloat(const char *name, float value) const {
        glUniform1f(getLocation(name), value);
    }

    void setMat4(const char *name, glm::mat4 value) const {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    void setVec3(const char *name, glm::vec3 value) const {
        glUniform3fv(getLocation(name), 1, &value[0]);
    }

    GLint getLocation(const char *name) const {
        return glGetUniformLocation(id, name);
    }

private:
    void checkError(unsigned int target, const char *type) {
        char errorLog[1024];
        int success;
        if (strcmp(type, "Shader") == 0) {
            glGetShaderiv(target, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(target, 1024, NULL, errorLog);
                printf("errorLog: %s", errorLog);
            }
        } else if (strcmp(type, "Program") == 0) {
            glGetProgramiv(target, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(target, 1024, NULL, errorLog);
                printf("errorLog: %s", errorLog);
            }
        }
    }
};

#endif //HIGHOPENGL_SHADER_H
