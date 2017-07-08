#ifndef PAC_MAN_PLAYER_H
#define PAC_MAN_PLAYER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include "Level.h"
#include "Mesh.h"

class Player {
public:
    Player(Level *levell);
    ~Player();
    void render(GLuint shaderProgram, glm::mat4 viewProject, glm::mat4 modelLevel, float deltaTime);
    void processInput(std::map< int, int > keyboard);
    void update(GLfloat deltaTime);
    void respawn();

    int type = 3;
    glm::vec2 moveTest(GLfloat deltaTime);
    float radius = 0.025;
    float speed = 0.25;
    glm::vec2 position;
private:
    bool moveToCenter(GLfloat distance);

    struct Bod{
        glm::vec3 pos;
        glm::vec3 normal;
    };
    Level *level;
    GLuint vbo,vao,ebo, vbo2, nbo, nbo2;
    glm::vec2 orientation;
    glm::vec2 levelPosition;
    glm::vec2 nextLevelCenter;
    glm::vec2 oldLevelCenter;
    glm::vec2 newOrientation;
    glm::vec2 memory = glm::vec2(0.0, 0.0);
    glm::mat4 model;
    const int KEY_FRAMES_COUNT = 3;
    std::vector<Mesh> keyFrames;
    std::vector<std::vector<Bod> > kf;

    const glm::vec2 STARTING_POSITION = glm::vec2(9.0, 12.0);
    const glm::vec2 STARTING_ORIENTATION = glm::vec2(0.0, 0.0);

    float t=0.0;
    float sum =0.0;
    int startingIndex = 0;
    int finalIndex = 1;
    int index = 0;
};


#endif
