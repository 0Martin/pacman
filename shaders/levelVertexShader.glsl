#version 330 core

in vec3 position;
in vec2 texPos;
in vec3 normal;

uniform mat4 proj;
uniform mat4 model;

out vec2 TexPos;
out vec3 Pos;
out vec3 Normal;


void main(){

    TexPos = texPos;
    Normal = vec3(model * vec4(normal, 1.0));
    Pos = position;

    gl_Position = proj  * model * vec4(position, 1.0f);
}
