#include "Scene.h"

float absValue(float value){
    if (value < 0.0)
        return value*(-1);
    else
        return value;
}

Scene::Scene() {

    enemies.push_back(Enemy(&level, &creatureMesh, glm::vec2(9.0, 8.0), glm::vec3(1.0, 0.0, 0.0)));
    enemies.push_back(Enemy(&level, &creatureMesh, glm::vec2(5.0, 8.0), glm::vec3(1.0, 0.54, 0.0)));
    enemies.push_back(Enemy(&level, &creatureMesh, glm::vec2(12.0, 8.0), glm::vec3(0.0, 1.0, 1.0)));
    enemies.push_back(Enemy(&level, &creatureMesh, glm::vec2(9.0, 10.0), glm::vec3(1.0, 0.71, 0.75)));


    lShader = new Shader("shaders/levelVertexShader.glsl", "shaders/levelFragmentShader.glsl");
    lShaderProgram = lShader->getProgram("outColor");

    pShader = new Shader("shaders/objectsVertexShader.glsl", "shaders/objectsFragmentShader.glsl");
    pShaderProgram = pShader->getProgram("outColor");

    playerShader = new Shader("shaders/playerVertexShader.glsl", "shaders/playerFragmentShader.glsl");
    playerShaderProgram = playerShader->getProgram("outColor");


    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
    viewProject =  proj * view;
}

Scene::~Scene() {
    lShader->~Shader();
    pShader->~Shader();
    playerShader->~Shader();
}

void Scene::render(float deltaTime){
    lShader->use();
    glUniform3f(glGetUniformLocation(lShaderProgram, "LightDir"), sunDir.x, sunDir.y, sunDir.z);
    uniformLights(lShaderProgram);
    level.render(lShaderProgram, viewProject);

    if(lives > 0) {
        playerShader->use();
        glUniform3f(glGetUniformLocation(pShaderProgram, "LightDir"), sunDir.x, sunDir.y, sunDir.z);
        player.render(playerShaderProgram, viewProject, model, deltaTime);
    }

    pShader->use();
    uniformLights(pShaderProgram);
    glUniform3f(glGetUniformLocation(pShaderProgram, "LightDir"), sunDir.x, sunDir.y, sunDir.z);
    glUniform3f(glGetUniformLocation(pShaderProgram, "emision"), 0.2, 0.2, 0.2);
    for(int i=0; i<enemies.size(); i++)
        enemies.at(i).render(pShaderProgram, viewProject, model);
    glUniform3f(glGetUniformLocation(pShaderProgram, "emision"), 0.0, 0.0, 0.0);
    for(int i=0; i<level.gems.size(); i++)
        level.gems.at(i).render(pShaderProgram, viewProject, model);
    for(int i=0; i<level.powerUps.size(); i++)
        level.powerUps.at(i).render(pShaderProgram, viewProject, model);

}


void Scene::update(float deltaTime){
    if (lives > 0) {
        player.update(deltaTime);
    }
    for(int i=0; i<enemies.size(); i++)
        enemies[i].Update(deltaTime);
    if(powerUp == 1){
        if(powerUpRemainingTime>0)
            powerUpRemainingTime -= deltaTime;
        else {
            powerUp = 0;
            for (int i=0; i<enemies.size(); i++)
                enemies.at(i).powerDown();
        }
    }
}

void Scene::processInput(std::map< int, int > mouse, double delta_cursor[2], std::map< int, int > keyboard){
    player.processInput(keyboard);
    model = level.processInput(mouse, delta_cursor);
}

void Scene::detectColision() {
    if(lives > 0) {
        for (int i = 0; i < level.gems.size(); i++) {
            if (absValue(player.position.x - level.gems[i].position.x) < player.radius + level.gems[i].radius
                && absValue(player.position.y - level.gems[i].position.y) < player.radius + level.gems[i].radius) {
                level.gems.erase(level.gems.begin() + i);
                score += 30;
                if (level.gems.size() == 0)
                    printf("Victory!!!!!!!!!!!!\n");
            }
        }

        for (int i = 0; i < level.powerUps.size(); i++) {
            if (absValue(player.position.x - level.powerUps[i].position.x) < player.radius + level.powerUps[i].radius
                &&
                absValue(player.position.y - level.powerUps[i].position.y) < player.radius + level.powerUps[i].radius) {
                level.powerUps.erase(level.powerUps.begin() + i);
                powerUp = 1;
                powerUpRemainingTime = powerUpLength;
                for (int i = 0; i < enemies.size(); i++)
                    enemies.at(i).powerUp();
            }
        }

        for (int i = 0; i < enemies.size(); i++) {
            if (enemies.at(i).dead == 0 &&
                absValue(player.position.x - enemies.at(i).position.x) < player.radius + enemies.at(i).radius &&
                absValue(player.position.y - enemies.at(i).position.y) < player.radius + enemies.at(i).radius)
                if (powerUp)
                    enemies.at(i).die();
                else if (lives > 0) {
                    lives -= 1;
                    player.respawn();
                }
        }
    }

}

void Scene::uniformLights(GLuint shaderProgram){
    char s[50];
    int count = 0;
    for(int i=0; i<enemies.size(); i++) {
        if (enemies.at(i).dead == 0) {
            sprintf(s, "pointLights[%d].position", count);
            glUniform3f(glGetUniformLocation(shaderProgram, s), enemies.at(i).position.x,
                        enemies.at(i).position.y, enemies.at(i).radius);
            sprintf(s, "pointLights[%d].lightColor", count);
            glUniform3f(glGetUniformLocation(shaderProgram, s), enemies.at(i).color.x, enemies.at(i).color.y,
                        enemies.at(i).color.z);
            count ++;
        }
    }

    glUniform1i(glGetUniformLocation(shaderProgram, "gemNum"), count);
}
