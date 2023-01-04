#pragma once

#include "shaderWrapper.h"

class shaderProgramWrapper
{
private:
    /* data */
    bool programCanBeDeleted = false; //this is needed to avoid deleting the program handle when moving
    //the program handle does not have a null value that can be used to check if its valid for deletion after moving
public:
    unsigned int shaderProgram;
    shaderProgramWrapper() = default;
    shaderProgramWrapper(shaderWrapper vertexShader, shaderWrapper fragmentShader);

    //rule of 5: copy deleted, move allowed
    shaderProgramWrapper(const shaderProgramWrapper&) = delete;
    shaderProgramWrapper& operator=(const shaderProgramWrapper&) = delete;
    //move has to avoid deleting the program handle
    shaderProgramWrapper(shaderProgramWrapper&& other) noexcept
    {
        programCanBeDeleted = other.programCanBeDeleted;
        other.programCanBeDeleted = false;
        shaderProgram = other.shaderProgram;
    }
    shaderProgramWrapper& operator=(shaderProgramWrapper&& other) noexcept
    {
        if(this != &other)
        {
            programCanBeDeleted = other.programCanBeDeleted;
            other.programCanBeDeleted = false;
            shaderProgram = other.shaderProgram;
        }
        return *this;
    }

    void use();
    ~shaderProgramWrapper();
};