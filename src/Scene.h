#ifndef PAC_MAN_OBJECTSHANDLER_H
#define PAC_MAN_OBJECTSHANDLER_H

#include "Player.h"
#include "Enemy.h"
#include "Gem.h"
#include "Shader.h"
#include "Mesh.h"


class Scene {
public:
    Scene();
    ~Scene();
    void detectColision();
    void render(float deltaTime);
    void update(float deltaTime);
    void processInput(std::map< int, int > mouse, double delta_cursor[2], std::map< int, int > keyboard);

private:
    void uniformLights(GLuint shaderProgram);

    bool powerUp = 0;
    float powerUpLength = 10;
    float powerUpRemainingTime;

    Mesh creatureMesh = Mesh("mashes/enemy.obj");
    Shader *lShader;
    Shader *pShader;
    Shader *playerShader;
    GLuint lShaderProgram;
    GLuint pShaderProgram;
    GLuint playerShaderProgram;

    Level level = Level();
    Player player = Player(&level);
    std::vector<Enemy> enemies;
    glm::mat4 viewProject;
    glm::vec3 sunDir = glm::vec3(0.2, 0.2, -0.5);
    glm::mat4 model;

    int score = 0;
    int lives = 3;


};


#endif
