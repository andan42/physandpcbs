#include "shaderProgramWrapper.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>


shaderProgramWrapper::shaderProgramWrapper(shaderWrapper vertexShader, shaderWrapper fragmentShader)
{
    programCanBeDeleted = true;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader.shaderID);
    glAttachShader(shaderProgram, fragmentShader.shaderID);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
    }
}
void shaderProgramWrapper::use()
{
    if(!programCanBeDeleted)
    {
        std::cout << "ERROR::SHADER::PROGRAM::USE_FAILED" << std::endl;
        //throw exception
        throw std::exception(); //this is not the best way to handle this
        //todo: find a better way to handle this and learn how to use exceptions in cpp properly <3

        return;
    }
    glUseProgram(shaderProgram);
}
shaderProgramWrapper::~shaderProgramWrapper()
{
    if(programCanBeDeleted)
    {
        glDeleteProgram(shaderProgram);
        //check if the program was deleted
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_DELETE_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::DELETION_FAILED" << infoLog << std::endl;
        }
    }
}
