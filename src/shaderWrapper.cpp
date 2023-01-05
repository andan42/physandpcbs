#include "shaderWrapper.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

void loadShader(const char* path, char** buffer)
{
    std::ifstream file;
    file.open(path);
    if(!file.is_open())
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    std::stringstream shaderStream;
    shaderStream << file.rdbuf();
    file.close();
    std::string shaderString = shaderStream.str();
    *buffer = new char[shaderString.length() + 1]; //why didnt you include the +1 for me copilot? :( jk ly
    strcpy(*buffer, shaderString.c_str());

}

shaderWrapper::shaderWrapper(const char* path, unsigned int shaderType)
{
    shaderCanBeDeleted = true;
    char* buffer;
    loadShader(path, &buffer);

    shaderID = glCreateShader(shaderType);

    glShaderSource(shaderID, 1, &buffer, NULL);
    glCompileShader(shaderID);

    int success; 
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << infoLog << std::endl;
    }

    delete[] buffer;

    buffer = new char[7];
    delete[] buffer;
}

shaderWrapper::~shaderWrapper()
{
    if (shaderCanBeDeleted)
    {       
         glDeleteShader(shaderID);
    }
}