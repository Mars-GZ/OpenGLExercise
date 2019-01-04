#version 330 core

uniform samplerCube skyBox;

in vec3 Texcoords;
out vec4 FragColor;

void main() {
    FragColor = textureCube(skyBox,Texcoords);
}
