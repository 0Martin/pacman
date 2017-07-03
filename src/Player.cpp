#include "Player.h"

Player::Player(Level *levell){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vbo2);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &nbo);
    glGenBuffers(1, &nbo2);


    level = levell;

    memory = glm::vec2(0.0, 0.0);

    Bod bod;
    std::vector<Bod> vektor;

    keyFrames.push_back(Mesh("mashes/keyFrame1.obj"));
    keyFrames.push_back(Mesh("mashes/keyFrame2.obj"));
    keyFrames.push_back(Mesh("mashes/keyFrame3.obj"));


    for(int j=0; j<KEY_FRAMES_COUNT; j++){
        kf.push_back(vektor);
        for(int i=0;i<keyFrames[j].out_vertices.size();i++){
            bod.pos = keyFrames[j].out_vertices.at(i);
            bod.normal = keyFrames[j].out_normals.at(i);
            kf.at(j).push_back(bod);
        }
    }



    levelPosition = STARTING_POSITION;
    position = level->getCenterOfLevelCoord(levelPosition.x, levelPosition.y);
    nextLevelCenter = position;
    orientation = STARTING_ORIENTATION;
}

Player::~Player(){
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
}

void Player::render(GLuint shaderProgram, glm::mat4 viewProject,  glm::mat4 modelLevel, float deltaTime){

    sum+=deltaTime;

    if(sum >= 0.25) {
        t = 1.0;
        sum = 0.0;
        index = (index + 1) % 4;


        if (index == 0){
            startingIndex = 0;
            finalIndex = 1 ;
        }
        if (index == 1){
            startingIndex = 1;
            finalIndex = 2;
        }
        if (index == 2){
            startingIndex = 2;
            finalIndex = 1;
        }
        if (index == 3){
            startingIndex = 1;
            finalIndex = 0;
        }
    }
    else
        t = sum*4;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, kf.at(startingIndex).size() * sizeof(Bod), kf.at(startingIndex).data(), GL_DYNAMIC_DRAW);

    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_elements.size() * sizeof(int), out_elements.data(), GL_STATIC_DRAW);*/

    GLint  possAtribute = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(possAtribute);
    glVertexAttribPointer(possAtribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint  normalAttribute = glGetAttribLocation(shaderProgram, "normal");
    glEnableVertexAttribArray(normalAttribute);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, kf.at(finalIndex).size() * sizeof(Bod), kf.at(finalIndex).data(), GL_DYNAMIC_DRAW);

    GLint  possAtribute2 = glGetAttribLocation(shaderProgram, "position2");
    glEnableVertexAttribArray(possAtribute2);
    glVertexAttribPointer(possAtribute2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    GLint  normalAttribute2 = glGetAttribLocation(shaderProgram, "normal2");
    glEnableVertexAttribArray(normalAttribute2);
    glVertexAttribPointer(normalAttribute2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

    GLint modelPos = glGetUniformLocation(shaderProgram, "model");
    glm::mat4 scale;
    scale = glm::scale(scale, glm::vec3(radius, radius, radius));
    glm::mat4 translate;
    translate = glm::translate(translate, glm::vec3(position, 0.1));
    glm::mat4 rotate;
    rotate = glm::rotate(rotate, (float)glm::radians((-orientation.x)*90.0+(-orientation.y-abs((int)orientation.y))*90.0), glm::vec3(0.0, 0.0, 1.0));
    model =  translate * rotate * scale;
    glUniformMatrix4fv(modelPos, 1, GL_FALSE ,glm::value_ptr(model));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "level"), 1, GL_FALSE, glm::value_ptr(modelLevel));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotate"), 1, GL_FALSE, glm::value_ptr(rotate));


    GLint projPos = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(projPos, 1, GL_FALSE, glm::value_ptr(viewProject));

    GLint colorPos = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(colorPos, 1.0, 1.0, 0.0);

    glUniform1f(glGetUniformLocation(shaderProgram, "t"), t);

    glDrawArrays(GL_TRIANGLES, 0, kf.at(startingIndex).size());

}

void Player::processInput(std::map< int, int > keyboard){
    if (keyboard[GLFW_KEY_UP])
        newOrientation = glm::vec2(0.0, -1.0);

    if (keyboard[GLFW_KEY_DOWN])
        newOrientation = glm::vec2(0.0, 1.0);

    if (keyboard[GLFW_KEY_RIGHT])
        newOrientation = glm::vec2(1.0, 0.0);

    if (keyboard[GLFW_KEY_LEFT])
        newOrientation = glm::vec2(-1.0, 0.0);

}

bool Player::moveToCenter(GLfloat distance){
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

void Player::update(GLfloat deltaTime){
    if (moveToCenter(deltaTime*speed)) {
        levelPosition = level->normalizeToLevelCoord(position);
        if (memory != glm::vec2(0.0, 0.0)) {
            newOrientation = memory;
            memory = glm::vec2(0.0, 0.0);
        }
        if (level->canmove(levelPosition, newOrientation)) {
            nextLevelCenter = level->getCenterOfLevelCoord(levelPosition.x + newOrientation.x, levelPosition.y + newOrientation.y);
            oldLevelCenter = level->getCenterOfLevelCoord(levelPosition.x, levelPosition.y);
            orientation = newOrientation;
        }

    }
    else{
        if (newOrientation * orientation == glm::vec2(0.0, 0.0)) {
            memory = newOrientation;
        }
        else if (orientation == -newOrientation) {
            orientation = newOrientation;
            glm::vec2 docasCenter = oldLevelCenter;
            oldLevelCenter = nextLevelCenter;
            nextLevelCenter = docasCenter;
        }
    }

}

void Player::respawn() {

    levelPosition = STARTING_POSITION;
    position = level->getCenterOfLevelCoord(levelPosition.x, levelPosition.y);
    nextLevelCenter = position;
    newOrientation = STARTING_ORIENTATION;
}


