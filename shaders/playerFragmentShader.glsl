#version 330 core

in vec3 DirLight;
in vec3 Normal;

uniform vec3 color;
uniform vec3  LightDir;

out vec4 outColor;

vec3 CalcDirLight(vec3 lightDir, vec3 Normal);

void main() {
    outColor =  vec4(color*CalcDirLight(LightDir, Normal), 1.0);
}

vec3 CalcDirLight(vec3 lightDir, vec3 normal){
    vec3 ambient = vec3(0.2, 0.2, 0.2);

    vec3 diffuse;
    vec3 lightVector = normalize(-lightDir);
    diffuse = max(dot(lightVector, normal), 0.0) * vec3(0.6, 0.6, 0.6);

    vec3 spec;
    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    vec3 reflectDir = reflect(lightDir, normal);
    spec = pow(max(dot(reflectDir, viewDir), 0.0), 5.0) * vec3(1.0, 1.0, 1.0);

    return diffuse+ambient+spec;
}