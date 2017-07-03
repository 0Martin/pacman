#include "Enemy.h"

Enemy::Enemy(Level *levell, Mesh *meshh,glm::vec2 Position, glm::vec3 Color){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    level = levell;
    mesh = meshh;

    Bod bod;
    for(int i=0; i<mesh->out_vertices.size(); i++){
        bod.pos=mesh->out_vertices[i];
        bod.normal=mesh->out_normals[i];
        out_body.push_back(bod);
    }

    startinPosition = Position;
    levelPosition = Position;
    position = level->getCenterOfLevelCoord(levelPosition.x, levelPosition.y);
    nextLevelCenter = position;
    orientation = STARTING_ORIENTATION;
    startingColor = Color;
    color = Color;
    srand (time(NULL));
}

Enemy::~Enemy(){
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Enemy::render(GLuint shaderProgram, glm::mat4 viewProject, glm::mat4 modelLevel){
    if (dead == 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, out_body.size() * sizeof(Bod), out_body.data(), GL_STATIC_DRAW);

        GLint possAtribute = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(possAtribute);
        glVertexAttribPointer(possAtribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

        GLint normalAttribute = glGetAttribLocation(shaderProgram, "normal");
        glEnableVertexAttribArray(normalAttribute);
        glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

        GLint modelPos = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 scale;
        scale = glm::scale(scale, glm::vec3(radius, radius, radius));
        glm::mat4 translate;
        translate = glm::translate(translate, glm::vec3(position, 0.1));
        glm::mat4 rotate;
        rotate = glm::rotate(rotate, (float)glm::radians((-orientation.x)*90.0+(-orientation.y-abs((int)orientation.y))*90.0), glm::vec3(0.0, 0.0, 1.0));
        model =  translate * rotate * scale;
        glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "level"), 1, GL_FALSE, glm::value_ptr(modelLevel));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotate"), 1, GL_FALSE, glm::value_ptr(rotate));

        GLint projPos = glGetUniformLocation(shaderProgram, "proj");
        glUniformMatrix4fv(projPos, 1, GL_FALSE, glm::value_ptr(viewProject));

        GLint colorPos = glGetUniformLocation(shaderProgram, "color");
        glUniform3f(colorPos, color.x, color.y, color.z);

        glDrawArrays(GL_TRIANGLES, 0, out_body.size());
    }
}

glm::vec2 Enemy::genRandomOrientation(){
    glm::vec2 newOrientation;
    int random = rand() % 4 + 1;

    newOrientation.x=(float) (((random+1) % 2) * (random-3));
    newOrientation.y=(float) ((random % 2) * (random-2));

    return newOrientation;
}


bool Enemy::moveToCenter(GLfloat distance){
    if (orientation.x == 1 && position.x+orientation.x*distance<nextLevelCenter.x){
        position += orientation*distance;
        return false;
    }
    else if (orientation.x == -1 && position.x+orientation.x*distance>nextLevelCenter.x){
        position += orientation*distance;
        return false;
    }
    else if (orientation.y == 1 && position.y-orientation.y*distance>nextLevelCenter.y){
        position -= orientation*distance;
        return false;
    }
    else if (orientation.y == -1 && position.y-orientation.y*distance<nextLevelCenter.y){
        position -= orientation*distance;
        return false;
    }
    else {
        position = nextLevelCenter;
        return true;
    }
}

bool Enemy::deadEnd(){
    bool answer = true;
    glm::vec2 oppositeOrientation = glm::vec2(0.0, 0.0) - orientation;

    answer &=( oppositeOrientation == glm::vec2(1.0, 0.0) || !(level->canmove(levelPosition, glm::vec2(1.0, 0.0))) );
    answer &=( oppositeOrientation == glm::vec2(-1.0, 0.0) || !(level->canmove(levelPosition, glm::vec2(-1.0, 0.0))) );
    answer &=( oppositeOrientation == glm::vec2(0.0, 1.0) || !(level->canmove(levelPosition, glm::vec2(0.0, 1.0))) );
    answer &=( oppositeOrientation == glm::vec2(0.0, -1.0) || !(level->canmove(levelPosition, glm::vec2(0.0, -1.0))) );


    return answer;
}

void Enemy::Update(GLfloat delatTime) {
    if (dead == 0) {
        if (moveToCenter(delatTime * speed)) {
            levelPosition = level->normalizeToLevelCoord(nextLevelCenter);

            glm::vec2 newOrientation = genRandomOrientation();
            while ((newOrientation == glm::vec2(0.0, 0.0) - orientation && !deadEnd())
                   || !(level->canmove(levelPosition, newOrientation)))
                newOrientation = genRandomOrientation();
            orientation = newOrientation;

            nextLevelCenter = level->getCenterOfLevelCoord((int) (levelPosition.x + orientation.x),
                                                           (int) (levelPosition.y + orientation.y));
        }
    } else {
        if (timeToRespawn - delatTime < 0) {
            respawn();
            timeToRespawn = 0.0;
        } else
            timeToRespawn -= delatTime;
    }
}

void Enemy::die() {
    dead = 1;
    timeToRespawn = respawnTime;
}

void Enemy::respawn() {
    dead = 0;

    levelPosition = startinPosition;
    position = level->getCenterOfLevelCoord(levelPosition.x, levelPosition.y);
    nextLevelCenter = position;
    orientation = STARTING_ORIENTATION;
}

void Enemy::powerDown(){
    color = startingColor;
    speed = originalSpeed;
}

void Enemy::powerUp() {
    speed = originalSpeed / 2;
    color = glm::vec3(0.0, 0.0, 1.0);
}