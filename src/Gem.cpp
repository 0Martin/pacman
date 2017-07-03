#include "Gem.h"

Gem::Gem(Mesh *meshh, glm::vec2 Position){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);


    mesh = meshh;

    Bod bod;
    for(int i=0; i<mesh->out_vertices.size(); i++){
        bod.pos=mesh->out_vertices[i];
        bod.normal=mesh->out_normals[i];
        out_body.push_back(bod);
    }
    position = Position;
    color = glm::vec3(0.8, 0.8, 0.5);
}

Gem::~Gem(){
}

void Gem::render(GLuint shaderProgram, glm::mat4 viewProject,  glm::mat4 modelLevel){
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, out_body.size() * sizeof(Bod), out_body.data(), GL_STATIC_DRAW);

    GLint  possAtribute = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(possAtribute);
    glVertexAttribPointer(possAtribute, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), 0);

    GLint  normalAttribute = glGetAttribLocation(shaderProgram, "normal");
    glEnableVertexAttribArray(normalAttribute);
    glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)(3*sizeof(float)));

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

  /*  GLint posPos = glGetUniformLocation(shaderProgram, "pos");
    glm::mat4 translate;
    translate = glm::translate(translate, glm::vec3(position, 0.1));
    glUniformMatrix4fv(posPos, 1, GL_FALSE, glm::value_ptr(translate));*/

    GLint projPos = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(projPos, 1, GL_FALSE, glm::value_ptr(viewProject));

    GLint colorPos = glGetUniformLocation(shaderProgram, "color");
    glUniform3f(colorPos, color.x, color.y, color.z);

    glDrawArrays(GL_TRIANGLES, 0, out_body.size());
}
