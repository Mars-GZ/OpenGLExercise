#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 Texcoords;

struct Material{
    vec3 ambient;//环境材料
    vec3 diffuse;//漫反射材料
    vec3 specular;//镜面反射材料
    float shininess;//反光度
};

//平行光源
struct DirLight{
    vec3 direction;//方向
    vec3 ambient;//环境光分量
    vec3 diff;//漫反射光分量
    vec3 specular;//镜面反射分量
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;

out vec4 FragColor;

vec3 getDirLight(DirLight);

void main() {
    FragColor = vec4(getDirLight(dirLight),1.0);
}

vec3 getDirLight(DirLight light){
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * material.ambient;

    float viewDiff = max(dot(norm,lightDir),0.0);
    vec3 diff = light.diff * viewDiff * material.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec*light.specular*
                    material.specular;
    return ambient + diff + specular;
}


