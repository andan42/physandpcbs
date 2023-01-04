#pragma once

class shaderWrapper
{
public:
    unsigned int shaderID;
    shaderWrapper(const char* path, unsigned int shaderType);
    ~shaderWrapper();	
};