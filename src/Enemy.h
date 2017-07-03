#ifndef PAC_MAN_ENEMY_H
#define PAC_MAN_ENEMY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <time.h>
#include "Level.h"
#include "Mesh.h"


class Enemy {
public:
    Enemy(Level *levell, Mesh *meshh,glm::vec2 Position, glm::vec3 Color);
    ~Enemy();
    void render(GLuint shaderProgram, glm::mat4 viewProject,  glm::mat4 modelLevel);
    void Update(GLfloat delatTime);
    void die();
    void powerDown();
    void powerUp();

    int type = 1;
    bool dead = 0;
    float radius = 0.025;
    float speed = 0.25;
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 levelPosition;
private:
    glm::vec2 genRandomOrientation();
    bool moveToCenter(GLfloat deltaTime);
    bool deadEnd();
    void respawn();

    struct Bod{
        glm::vec3 pos;
        glm::vec3 normal;
    };
    Level *level;
    Mesh *mesh;
    GLuint vbo,vao,ebo;
    glm::vec2 orientation;
    glm::vec2 nextLevelCenter;
    glm::mat4 model;
    std::vector<Bod> out_body;

    const glm::vec2 STARTING_ORIENTATION = glm::vec2(0.0, 0.0);
    float respawnTime = 5.0;
    float timeToRespawn;
    glm::vec2 startinPosition;
    glm::vec3 startingColor;
    float originalSpeed = 0.25;
};


#endif
