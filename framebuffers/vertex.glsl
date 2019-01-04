#version 330 core

layout(location = 0)in vec3 xPos;
layout(location = 1)in vec2 xTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 Texcoords;

void main() {
    Texcoords = vec2(xTexCoords);
    gl_Position = projection*view*model*vec4(xPos,1);
}
