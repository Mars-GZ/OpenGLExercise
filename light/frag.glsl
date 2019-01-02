#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 Texcoords;

struct Material{
    sampler2D ambient;//环境材料
    sampler2D diffuse;//漫反射材料
    sampler2D specular;//镜面反射材料
    float shininess;//反光度
};

//平行光源
struct DirLight{
    vec3 direction;//方向
    vec3 ambient;//环境光分量
    vec3 diff;//漫反射光分量
    vec3 specular;//镜面反射分量
};

//普通光源
struct Light{
    vec3 position;//位置
    vec3 direction;//方向

    vec3 ambient;//环境光分量
    vec3 diff;//漫反射光分量
    vec3 specular;//镜面反射分量

    float constant;
    float linear;
    float quad;

    //聚光 对应的内切光角与外切光角
    float inCutOff;
    float outCutOff;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
uniform Light spotLight;

out vec4 FragColor;

vec3 getDirLight(DirLight);

vec3 getSpotLight(Light);

void main() {
//    FragColor = vec4(getDirLight(dirLight),1.0);
    FragColor = vec4((getSpotLight(spotLight)),1.0);
}

vec3 getDirLight(DirLight light){
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient * texture(material.ambient,Texcoords).rgb;

    float viewDiff = max(dot(norm,lightDir),0.0);
    vec3 diff = light.diff * viewDiff * texture(material.diffuse,Texcoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec*light.specular*
                    texture(material.specular,Texcoords).rgb;
    return ambient + diff + specular;
}

vec3 getSpotLight(Light light){
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    vec3 ambient = light.ambient * texture(material.ambient,Texcoords).rgb;

    float viewDiff = max(dot(norm,lightDir),0.0);
    vec3 diff = light.diff * viewDiff * texture(material.diffuse,Texcoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = spec*light.specular*
                    texture(material.specular,Texcoords).rgb;

    //随着距离的变化，光源的强度会衰减
    float distance = length(light.position-FragPos);
    float attention = 1.0/(light.constant + light.linear * distance +
                        light.quad * distance * distance);

    float theta = dot(lightDir,normalize(-light.direction));
    float epsilon = light.inCutOff-light.outCutOff;
    float intensity = clamp((theta-light.outCutOff)/epsilon,0.0,1.0);
    return (ambient + diff + specular)*attention + (diff+specular)*intensity;
}


