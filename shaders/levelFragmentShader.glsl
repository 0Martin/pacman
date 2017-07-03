#version 330 core

in vec3 Pos;
in vec2 TexPos;
in vec3 Normal;


uniform sampler2D tex;
uniform sampler2D floor;
uniform vec3  LightDir;

out vec4 outColor;

struct PointLight {
    vec3 position;
    vec3 lightColor;
};
uniform int gemNum;
#define MAX_LIGHTS 400
uniform PointLight pointLights[MAX_LIGHTS];

vec3 calcLight(vec3 normal, PointLight light, vec3 pos);
vec3 CalcDirLight(vec3 lightDir);

void main(){

        vec3 resultLight = vec3(0.0, 0.0, 0.0);

        vec3 DirLight =  CalcDirLight(LightDir);

        float distance;
        for(int i=0;i<gemNum;i++){
            distance = length(pointLights[i].position - Pos);
            if(distance<0.16)
            resultLight += calcLight(Normal, pointLights[i], Pos)*pointLights[i].lightColor;
        }

        if (Pos.z == 0.0)
            outColor = vec4(DirLight+resultLight+vec3(0.2, 0.2, 0.2), 1.0) * texture(floor, TexPos);
        else
            outColor = vec4(DirLight+resultLight, 1.0) * texture(tex, TexPos);

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
        float attenuation = 1.0 / (0.3 + distance*300.0*distance) ;

        return (diffuse+spec+ambient) * attenuation;
}

vec3 CalcDirLight(vec3 lightDir){
    vec3 ambient = vec3(0.2, 0.2, 0.2);

    vec3 diffuse;
    vec3 lightVector = normalize(-lightDir);
    diffuse = max(dot(lightVector, Normal), 0.0) * vec3(0.6, 0.6, 0.6);

    vec3 spec;
    vec3 viewDir = vec3(0.0, 0.0, -1.0);
    vec3 reflectDir = reflect(lightDir, Normal);
    spec = pow(max(dot(reflectDir, viewDir), 0.0), 5.0) * vec3(1.0, 1.0, 1.0);

    return diffuse+ambient+spec;
}