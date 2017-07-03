#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 model;
uniform mat4 pos;
uniform mat4 proj;
uniform mat4 level;
uniform mat4 rotate;

out vec3 Normal;
out vec3 Pos;



void main() {

    Normal = vec3(level * rotate * vec4(normal, 1.0));

   Pos = vec3( level  * model * vec4(position, 1.0));

    gl_Position = proj * level  * model * vec4(position, 1.0f);

}


