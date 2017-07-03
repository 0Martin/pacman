#include <iostream>
#define GLEW_BUILD
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include "Scene.h"

const GLint WIDTH = 800, HEIGHT = 600;

std::map< int, int > keyboard;
std::map< int, int > mouse;
double cursor[2] = {0.0, 0.0};
double delta_cursor[2]  = {0.0, 0.0};



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keyboard[key] = action;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouse[button] = action;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    delta_cursor[0] = xpos - cursor[0];
    delta_cursor[1] = ypos - cursor[1];
    cursor[0] = xpos;
    cursor[1] = ypos;
}

int main() {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Pac Man", nullptr, nullptr);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to inicialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    Scene scene;

    float actualTime = glfwGetTime();
    float lastTime = actualTime;
    float deltaTime = lastTime - glfwGetTime();



    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        actualTime = glfwGetTime();
        deltaTime = actualTime - lastTime;

        glfwPollEvents();

        scene.processInput(mouse, delta_cursor, keyboard);
        scene.update(deltaTime);
        scene.detectColision();
        scene.render(deltaTime);


        lastTime = actualTime;
        delta_cursor[0] = 0.0;
        delta_cursor[1] = 0.0;

        glfwSwapBuffers(window);
    }



    glfwTerminate();


    return 0;
}