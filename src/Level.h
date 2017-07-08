#ifndef PAC_MAN_LEVEL_H
#define PAC_MAN_LEVEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <math.h>
#include "Gem.h"
#include "Mesh.h"
#include "Map.h"
#include "PowerUp.h"


class Level {
public:
    std::vector<Gem> gems;
    std::vector<PowerUp> powerUps;

    Level();
    ~Level();
    void render(GLuint shaderProgram, glm::mat4 viewProject);
    glm::mat4 processInput(std::map< int, int > mouse, double delta_cursor[2]);

    char type = 3;
    glm::vec2 getCenterOfLevelCoord(int width, int height);
    glm::vec2 getCenterOfGemCoord(int width, int height);
    bool canmove(glm::vec2 position, glm::vec2 direction);
    glm::vec2 normalizeToLevelCoord(glm::vec2 bod);
private:
    struct Bod{
        glm::vec3 pos;
        glm::vec2 tex;
        glm::vec3 normal;
    };
    Mesh mesh = Mesh("mashes/level.obj");
    Map spaceMap, gemMap;
    float x, y;
    float minX, maxX, minY, maxY, sirka, vyska;
    float gemMinX, gemMinY, gemMaxX, gemMaxY, gemSirka, gemVyska;
    glm::vec2 spaceElementSize, gemElementSize;
    GLuint vbo,vao,ebo,tbo, tex, floor;
    unsigned char* wallImg;
    unsigned char* floorImg;
    int wallTexWidth, wallTexHeight, floorTexWidth, floorTexHeight;
    glm::mat4 model;
    std::vector<Bod> out_body;
};


#endif
