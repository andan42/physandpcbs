#pragma once

class shaderWrapper
{
private:
    bool shaderCanBeDeleted = false;
public:
    unsigned int shaderID;
    shaderWrapper(const char* path, unsigned int shaderType);

    //delete copy constructor and assignment operator
    shaderWrapper(const shaderWrapper&) = delete;
    shaderWrapper& operator=(const shaderWrapper&) = delete;
    //move constructor and assignment operator
    shaderWrapper(shaderWrapper&& other) noexcept{
        shaderCanBeDeleted = other.shaderCanBeDeleted;
        other.shaderCanBeDeleted = false;
        shaderID = other.shaderID;
    }
    shaderWrapper& operator=(shaderWrapper&& other) noexcept{
        if(this != &other)
        {
            shaderCanBeDeleted = other.shaderCanBeDeleted;
            other.shaderCanBeDeleted = false;
            shaderID = other.shaderID;
        }
        return *this;
    }
    ~shaderWrapper();	
};