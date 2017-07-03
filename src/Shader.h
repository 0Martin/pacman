#ifndef REALNY_SHADER_H
#define REALNY_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>


class Shader {
public:
    Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath);
    ~Shader();
    GLuint getProgram(const std::string fragDataLocation);
    void use();
private:
    GLuint shaderProgram;
    GLuint vertexShader;
    GLuint fragmentShader;
};


#endif
