#version 330 core

in vec3 position;
in vec3 normal;
in vec3 position2;
in vec3 normal2;

uniform mat4 model;
uniform mat4 pos;
uniform mat4 proj;
uniform mat4 level;
uniform mat4 rotate;
uniform float t;

out vec3 Normal;

void main() {

    vec3 Position = mix(position, position2, t);
    Normal = vec3(level * rotate * vec4(normal, 1.0));


    gl_Position = proj * level  * model * vec4(Position, 1.0f);

}
