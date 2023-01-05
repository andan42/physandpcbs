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
    //get how many attached shaders
    int numberOfShaders;
    glGetProgramiv(shaderProgram, GL_ATTACHED_SHADERS, &numberOfShaders);
}
void shaderProgramWrapper::use()
{
    if(!programCanBeDeleted)
    {
        std::cout << "this is not a program kekw" << std::endl;
        //throw exception
        throw std::exception(); //this is not the best way to handle this
        //todo: find a better way to handle this and learn how to use exceptions in cpp properly <3

        return;
    }
    //check for ANY opengl errors and print them to the console

    glUseProgram(shaderProgram);
}
shaderProgramWrapper::~shaderProgramWrapper()
{
    if(programCanBeDeleted)
    {
        glDeleteProgram(shaderProgram);
        //check if the program was deleted
        
    }
}
