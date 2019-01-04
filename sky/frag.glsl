#version 330 core

in VS_OUT{
vec3 Normal;
vec3 Position;
vec2 Texcoords;
} fs_in;

uniform vec3 cameraPos;
uniform samplerCube texture1;

out vec4 FragColor;

void main() {
    float ratio = 1.00/2;
    vec3 I = vec3(fs_in.Position - cameraPos);
    vec3 R = refract(I,normalize(fs_in.Normal),ratio);
    FragColor = texture(texture1,R);
    gl_FragDepth = 0.0;
}
