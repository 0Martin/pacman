#ifndef PAC_MAN_GEM_H
#define PAC_MAN_GEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <time.h>
#include "Mesh.h"

class Gem {
public:
    Gem(Mesh *meshh, glm::vec2 Position);
    ~Gem();
    void render(GLuint shaderProgram, glm::mat4 viewProject,  glm::mat4 modelLevel);

    int type = 2;
    float radius = 0.01;
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 orientation = glm::vec2(0.0, 0.0);
private:
    struct Bod{
        glm::vec3 pos;
        glm::vec3 normal;
    };
    Mesh *mesh;
    GLuint vbo,vao,ebo;
    glm::mat4 model;
    std::vector<Bod> out_body;
};


#endif
