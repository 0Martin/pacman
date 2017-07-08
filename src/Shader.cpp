#include "Shader.h"

Shader::Shader(const std::string vertexShaderPath, const std::string fragmentShaderPath){
    const GLchar *vertexCode;
    const GLchar *fragCode;
    std::ifstream vertexFile;
    std::ifstream fragFile;

    vertexFile.open(vertexShaderPath.c_str());
    if (!vertexFile.is_open())
        std::cout << "Failed to open vertex shader" << std::endl;
    fragFile.open(fragmentShaderPath.c_str());
    if (!fragFile.is_open())
        std::cout << "Failed to open fragment shader" << std::endl;

    std::string vertexShaderString((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
    std::string fragmentShaderString((std::istreambuf_iterator<char>(fragFile)), std::istreambuf_iterator<char>());
    vertexCode = vertexShaderString.c_str();
    fragCode = fragmentShaderString.c_str();

    GLint status;
    char buffer[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
        std::cout << vertexShaderPath <<" Vertex shader error: " << buffer << std::endl;
    }


    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
        std::cout << fragmentShaderPath << " Fragment shader error: " << buffer << std::endl;
    }
}

Shader::~Shader(){
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteProgram(vertexShader);
}

GLuint Shader::getProgram(const std::string fragDataLocation){
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, fragDataLocation.c_str());

    glLinkProgram(shaderProgram);

    return shaderProgram;
}

void Shader::use(){
    glUseProgram(shaderProgram);
}

