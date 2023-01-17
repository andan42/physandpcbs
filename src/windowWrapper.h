#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
class windowWrapper
{
public:
    const unsigned int SCR_WIDTH;// = ;
    const unsigned int SCR_HEIGHT;// = ;
    GLFWwindow* window;
    windowWrapper(const char* title, unsigned int width = 1000, unsigned int height = 1000);
    void makeCurrent();
    ~windowWrapper();
};
