#version 330 core

in vec3 Normal;
in vec3 Pos;

uniform vec3 color;
uniform vec3 LightDir;
uniform vec3 emision;

out vec4 outColor;

struct PointLight {
    vec3 position;
    vec3 lightColor;
};
uniform int gemNum;
#define MAX_LIGHTS 400
uniform PointLight pointLights[MAX_LIGHTS];

vec3 CalcDirLight(vec3 lightDir, vec3 Normal);
vec3 calcLight(vec3 normal, PointLight light, vec3 pos);

void main() {

    vec3 resultLight = vec3(0.0, 0.0, 0.0);

    vec3 DirLight =  CalcDirLight(LightDir, Normal);

    float distance;
    for(int i=0;i<gemNum;i++){
       distance = length(pointLights[i].position - Pos);
       if(distance<0.13 && distance >0.025)
         resultLight += calcLight(Normal, pointLights[i], Pos)*pointLights[i].lightColor;
    }

    outColor =  vec4(color *(DirLight+resultLight+emision), 1.0);
}

vec3 calcLight(vec3 normal, PointLight light, vec3 pos){

        vec3 ambient = vec3(0.2, 0.2, 0.2);

        vec3 diffuse;
        vec3 lightVector = normalize(light.position - pos);
        normal = normalize(normal);
        diffuse = max(dot(lightVector, normal), 0.0) * vec3(0.6, 0.6, 0.6);

        vec3 spec;
        vec3 viewDir = vec3(0.0, 0.0, 1.0);
        vec3 reflectDir = reflect(-lightVector, normal);
        spec = pow(max(dot(reflectDir, viewDir), 0.0), 5.0) * vec3(1.0, 1.0, 1.0);

        float distance = length(light.position - pos);
        float attenuation = 1.0 / (1.0 + distance*1.0) ;

        return (diffuse+spec+ambient) * attenuation;
}

vec3 CalcDirLight(vec3 lightDir, vec3 normal){
    vec3 ambient = vec3(0.2, 0.2, 0.2);

    vec3 diffuse;
    vec3 lightVector = normalize(-lightDir);
    diffuse = max(dot(lightVector, Normal), 0.0) * vec3(0.6, 0.6, 0.6);

    vec3 spec;
    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    vec3 reflectDir = reflect(lightDir, Normal);
    spec = pow(max(dot(reflectDir, viewDir), 0.0), 5.0) * vec3(1.0, 1.0, 1.0);

    return diffuse+ambient+spec;
}