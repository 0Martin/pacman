#ifndef PAC_MAN_MESH_H
#define PAC_MAN_MESH_H

#include<stdio.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>

class Mesh {
public:
    Mesh(const std::string path);
    ~Mesh();
    void render(GLuint shaderProgram, glm::mat4 viewProject);

    std::vector < glm::vec3 > out_vertices;
    std::vector < glm::vec2 > out_uvs;
    std::vector < glm::vec3 > out_normals;
    std::vector <int> out_elements;
    float limits[4];
private:
    bool loadObj(const std::string path);
};



#endif
