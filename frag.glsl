#version 330 core

in vec2 Texcoords;

uniform sampler2D grass;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(grass,Texcoords);
//    if(texColor.a<0.1){
//        //丢弃片段
//        discard;
//    }else{
       FragColor = texColor;
//    }
}


