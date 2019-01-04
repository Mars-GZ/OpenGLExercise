#version 330 core

in vec2 Texcoords;

uniform sampler2D texture1;

out vec4 FragColor;

void main() {
    FragColor = texture(texture1,Texcoords);
}
