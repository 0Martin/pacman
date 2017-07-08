#ifndef PAC_MAN_MAP_H
#define PAC_MAN_MAP_H

#include<stdio.h>
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

class Map {
public:
    Map();
    ~Map();
    bool loadMap(const std::string path);
    int getElement(int width, int height);

    int arrayWidth, arrayHeight;
private:
    int* array;
};


#endif
