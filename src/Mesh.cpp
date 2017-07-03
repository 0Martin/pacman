#include "Mesh.h"

Mesh::Mesh(const std::string path){
    loadObj(path);
}

Mesh::~Mesh(){

}

bool Mesh::loadObj(const std::string path) {

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    float minX = 1, maxX = -1, minY = 1, maxY = -1;

    printf("Loading OBJ file %s...\n", path.c_str());

    FILE * file = fopen(path.c_str(), "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }

    while( 1 ) {

        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        if ( strcmp( lineHeader, "v" ) == 0 ) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.z, &vertex.y);
            temp_vertices.push_back(vertex);
            if (vertex.x < minX)
                minX=vertex.x;
            if (vertex.x > maxX)
                maxX=vertex.x;
            if (vertex.y < minY)
                minY=vertex.y;
            if (vertex.y > maxY)
                maxY=vertex.y;
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 ) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 ) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.z, &normal.y);
            temp_normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 ) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            /*int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &normalIndex[1], &vertexIndex[2],
                                 &normalIndex[2]);*/
            if (matches != 9){
                printf("Wrong number of arguments for faces\n");
                return false;
            }
            out_elements.push_back(vertexIndex[0]-1);
            out_elements.push_back(vertexIndex[1]-1);
            out_elements.push_back(vertexIndex[2]-1);
            out_vertices.push_back(temp_vertices[vertexIndex[0]-1]);
            out_vertices.push_back(temp_vertices[vertexIndex[1]-1]);
            out_vertices.push_back(temp_vertices[vertexIndex[2]-1]);
            out_normals.push_back(temp_normals[normalIndex[0]-1]);
            out_normals.push_back(temp_normals[normalIndex[1]-1]);
            out_normals.push_back(temp_normals[normalIndex[2]-1]);
            out_uvs.push_back(temp_uvs[uvIndex[0]-1]);
            out_uvs.push_back(temp_uvs[uvIndex[1]-1]);
            out_uvs.push_back(temp_uvs[uvIndex[2]-1]);
        }
        else{
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    if (limits != NULL) {
        limits[0] = minX;
        limits[1] = maxX;
        limits[2] = minY;
        limits[3] = maxY;
    }

    fclose(file);
    return true;
}

void Mesh::render(GLuint shaderProgram, glm::mat4 viewProject){

}