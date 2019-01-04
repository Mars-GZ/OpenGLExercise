#version 330 core

layout(std140) uniform ExampleBlock{
    float value;
    vec3 vector;
    mat4 matrix;
    float values[3];
    bool boolean;
    int integer;
};

layout(location = 0)in vec3 xPos;
layout(location = 1)in vec2 xTexCoords;
layout(location = 2)in vec3 xNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
vec3 Normal;
vec3 Position;
vec2 Texcoords;
} vs_out;

void main() {
    vs_out.Normal = mat3(transpose(inverse(model)))*xNormal;
    vs_out.Texcoords = vec2(xTexCoords);
    vs_out.Position = vec3(model*vec4(xPos,1));
    gl_Position = projection*view*model*vec4(xPos,1);
}
