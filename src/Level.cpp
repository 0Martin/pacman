#include "Level.h"

Level::Level(){

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &tbo);
    glGenBuffers(1, &ebo);
    glGenTextures(1, &tex);
    glGenTextures(1, &floor);

    spaceMap.loadMap("maps/level.txt");
    gemMap.loadMap("maps/gems.txt");


    minX = mesh.limits[0];
    maxX = mesh.limits[1];
    minY = mesh.limits[2];
    maxY = mesh.limits[3];
    sirka = maxX - minX;
    vyska = maxY - minY;

    spaceElementSize.x = sirka / spaceMap.arrayWidth;
    spaceElementSize.y = vyska / spaceMap.arrayHeight;

    gemMinX = minX + spaceElementSize.x+0.02;
    gemMaxX = maxX - spaceElementSize.x - 0.02;
    gemMinY = minY + spaceElementSize.y + 0.02;
    gemMaxY = maxY - spaceElementSize.y - 0.02;
    gemSirka = gemMaxX - gemMinX;
    gemVyska = gemMaxY - gemMinY;

    gemElementSize.x = gemSirka / gemMap.arrayWidth;
    gemElementSize.y = gemVyska / gemMap.arrayHeight;

    x=0.0;
    y=0.0;

     Bod bod;
     for(int i=0; i<mesh.out_vertices.size(); i++){
         bod.pos=mesh.out_vertices[i];
         bod.tex=mesh.out_uvs[i];
         bod.normal=mesh.out_normals[i];
         out_body.push_back(bod);
     }


    Mesh powerUpMesh = Mesh("mashes/powerUp.obj");
    Mesh gemMesh = Mesh("mashes/gem.obj");

     for (int i=0; i<gemMap.arrayHeight;i++)
         for (int j = 0; j < gemMap.arrayWidth; j++){
             if (gemMap.getElement(j, i) == 1) {
                 gems.push_back(Gem(&gemMesh, getCenterOfGemCoord(j, i)));
             }
            else if(gemMap.getElement(j, i) == 2) {
                powerUps.push_back(PowerUp(&powerUpMesh, getCenterOfGemCoord(j, i)));
             }
         }

     wallImg = SOIL_load_image("textures/wall.jpg", &wallTexWidth, &wallTexHeight, 0, SOIL_LOAD_RGB);
     floorImg = SOIL_load_image("textures/floor.jpg", &floorTexWidth, &floorTexHeight, 0, SOIL_LOAD_RGB);
}

Level::~Level(){
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
    SOIL_free_image_data(wallImg);
    SOIL_free_image_data(floorImg);
}


void Level::render(GLuint shaderProgram, glm::mat4 viewProject){

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, out_body.size() * sizeof(Bod), out_body.data(), GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallTexWidth, wallTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, wallImg);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floorTexWidth, floorTexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, floorImg);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint  possAtribute = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(possAtribute);
    glVertexAttribPointer(possAtribute, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), 0);

    GLint  texAtribute = glGetAttribLocation(shaderProgram, "texPos");
    glEnableVertexAttribArray(texAtribute);
    glVertexAttribPointer(texAtribute, 2, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(3*sizeof(float)));

    GLint  normalAttribute = glGetAttribLocation(shaderProgram, "normal");
    glEnableVertexAttribArray(normalAttribute);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(5*sizeof(float)));

    GLint projPos = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(projPos, 1, GL_FALSE, glm::value_ptr(viewProject));


    GLint modelPos = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelPos, 1, GL_FALSE ,glm::value_ptr(model));

    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "floor"), 1);


    glDrawArrays(GL_TRIANGLES, 0 ,out_body.size());
}

glm::vec2 Level::normalizeToLevelCoord(glm::vec2 bod){
    glm::vec2 vysledok;

    vysledok.x = (int)((bod.x - minX) / sirka * 19.0);
    vysledok.y = (int)((maxY - bod.y) / vyska * 22.0);

    return vysledok;
}

glm::vec2 Level::getCenterOfLevelCoord(int width, int height){
    glm::vec2 center;

    if (width == -1 && height == 10)
        return getCenterOfLevelCoord(18, 10);
    else if (width == 19 && height == 10)
        return getCenterOfLevelCoord(0, 10);
    else {
        center.x = (float) ((width + 0.5) * (sirka / spaceMap.arrayWidth) + minX);
        center.y = (float) (maxY - (height + 0.5) * (vyska / spaceMap.arrayHeight));

        return center;
    }
}

glm::vec2 Level::getCenterOfGemCoord(int width, int height) {
    glm::vec2 center;

        center.x = (float) ((width + 0.5) * (gemSirka / gemMap.arrayWidth) + gemMinX);
        center.y = (float) (gemMaxY - (height + 0.5) * (gemVyska / gemMap.arrayHeight));

        return center;
}

bool Level::canmove(glm::vec2 position, glm::vec2 direction){
    if (position == glm::vec2(0.0, 10.0) && direction == glm::vec2(-1.0, 0.0))
        return true;
    else if (position == glm::vec2(18.0, 10.0) && direction == glm::vec2(1.0, 0.0))
        return true;
    else
        spaceMap.getElement((int)(position.x+direction.x), (int)(position.y+direction.y));
}


glm::mat4 Level::processInput(std::map< int, int > mouse, double delta_cursor[2]){
    if (mouse[GLFW_MOUSE_BUTTON_LEFT]) {
        x+= delta_cursor[0];
        y+= delta_cursor[1];

        glm::mat4 rotate;
        rotate = glm::rotate(rotate, (float)glm::radians(x/3.0), glm::vec3(0.0, 1.0, 0.0));
        rotate = glm::rotate(rotate, (float)glm::radians(y/3.0), glm::vec3(1.0, 0.0, 0.0));
        model = rotate;
    }

    return model;
}