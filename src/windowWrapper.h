#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
class windowWrapper
{
public:
    const unsigned int SCR_WIDTH;// = 800;
    const unsigned int SCR_HEIGHT;// = 600;
    GLFWwindow* window;
    windowWrapper(const char* title, unsigned int width = 800, unsigned int height = 600);
    void makeCurrent();
    ~windowWrapper();
};
