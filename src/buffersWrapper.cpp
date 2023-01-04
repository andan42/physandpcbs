#include "buffersWrapper.h"

#include <iostream>

GLuint buffersWrapper::boundVAO = 0;
buffersWrapper::buffersWrapper()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}
void buffersWrapper::bindVao()
{
    if(VAO != boundVAO) //
    {
        glBindVertexArray(VAO);
        boundVAO = VAO;
    }
    return;
}
//<3
//this assumes unsigned int type data <3 <3 <3
void buffersWrapper::setEbo(GLuint* data, int dataSize, GLenum usage)
{
    bindVao();
    if(eboCount == 0) //if the VAO doesnt have an EBO, then create one
    {
        glGenBuffers(1, &EBO); 
    }
    eboCount = dataSize / sizeof(GLuint); //update the size of the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, usage); //set the data
    return;
}
//<3 
//this assumes float type data <3 <3 <3
void buffersWrapper::addBuffer(int attrLocation, int attrSize, GLfloat* data, int dataSize, GLenum bufferType, GLenum usage, bool normalize)
{
    bindVao();
    //find if the VAO already has a buffer with the same attribute location
    int index = -1;
    for(int i = 0; i < attrLocations.size(); i++)
    {
        if(attrLocations[i] == attrLocation)
        {
            index = i;
            break;
        }
    }
    if(index != -1) //if the VAO already has a buffer with the same attribute location, try to set buffer instead of adding new
    {
        //check if the buffer with given attribute has the same attrsize, bufferType, usage and normalize
        if(attrSizes[index] == attrSize && bufferTypes[index] == bufferType && bufferUsages[index] == usage && normalizes[index] == normalize)
        {
            updateBuffer(attrLocation, data, dataSize);
            return;
        }
        else //if the buffer with given attribute has different attrsize, bufferType, usage or normalize, then throw an error
        {
            std::cout << "ERROR: the buffer with given attribute has different attrsize, bufferType, usage or normalize" << std::endl;
            throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
            return;
        }

        return;
    }
    if(attrSize < 1 || attrSize > 4) //if the attribute size is less than 1 or greater than 4, then throw an error
    {
        std::cout << "ERROR: the attribute size is less than 1 or greater than 4" << std::endl;
        throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
        return;
    }
    if(vertexCount == 0) //if the VAO doesnt have any VBOs OR size was reset, then set the vertex count
    {
        vertexCount = dataSize / (attrSize * sizeof(GLfloat));
    }
    else if(vertexCount != dataSize / (attrSize * sizeof(GLfloat))) //if the VAO already has VBOs, then check if the new VBO has the same number of vertices as the other VBOs
    {
        std::cout << "ERROR: the new buffer has a different number of vertices than the other buffers" << std::endl;
        throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
        return;
    }
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(bufferType, VBO);
    glBufferData(bufferType, dataSize, data, usage);  
    glVertexAttribPointer(attrLocation, attrSize, GL_FLOAT, normalize, attrSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attrLocation);
    VBOs.push_back(VBO);
    bufferTypes.push_back(bufferType);
    bufferUsages.push_back(usage);

    vertexCounts.push_back(vertexCount);

    normalizes.push_back(normalize);
    attrLocations.push_back(attrLocation);
    attrSizes.push_back(attrSize);
    return;
}
void buffersWrapper::updateBuffer(int attrLocation, GLfloat* data, int dataSize)
{
    bindVao();
    //find the buffer with the given attrLocation
    int index = -1;
    for(int i = 0; i < attrLocations.size(); i++)
    {
        if(attrLocations[i] == attrLocation)
        {
            index = i;
            break;
        }
    }
    if(index == -1) //throw error, no buffer with the given attribute location exists
    {
        std::cout << "ERROR: no buffer with the given attribute location exists" << std::endl;
        throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
        return;
    }
    if(vertexCount == 0) //if the VAO doesnt have any VBOs OR the size was reset, then set the vertex count
    {
        vertexCount = dataSize / (attrSizes[index] * sizeof(GLfloat));
    }
    else if(vertexCount != dataSize / (attrSizes[index] * sizeof(GLfloat))) //if the VAO already has VBOs, then check if the new VBO data has the same number of vertices as the old VBO data
    {
        std::cout << "ERROR: the new buffer data has a different number of vertices than the old buffer" << std::endl;
        throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
        return;
    }
    
    vertexCounts[index] = vertexCount;
    glBindBuffer(bufferTypes[index], VBOs[index]);
    glBufferData(bufferTypes[index], dataSize, data, bufferUsages[index]);
    return;
}
void buffersWrapper::resetBuffersSize()
{
    vertexCount = 0;
    return;
}
bool buffersWrapper::validateBufferSizes()
{
    for(int i = 0; i < vertexCounts.size(); i++)
    {
        if(vertexCounts[i] != vertexCount)
        {
            return false;
        }
    }
    return true;
}
//the destructor binds its VAO, and deletes its EBO, and deletes its VBOs, and deletes the VAO <3 <3 <3 <3
void buffersWrapper::draw()
{
    bindVao();
    if(!validateBufferSizes()) //if the VAO has VBOs, then check if the VBOs have the same number of vertices
    {
        std::cout << "ERROR: the VBOs have different numbers of vertices" << std::endl;
        throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3 
        return;
    }
    if(eboCount == 0) //if the VAO doesnt have an EBO, then draw the VAO as a GL_ARRAY_BUFFER
    {
        //throw std::exception(); //TODO learn how to actually throw errors in c++ <3 <3 <3
        //TODO keep track of the number of vertices in the VAO, and use that number instead of 0
        //<3 <3 <3
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }
    else //if the VAO has an EBO, then draw the VAO as a GL_ELEMENT_ARRAY_BUFFER
    {
        glDrawElements(GL_TRIANGLES, eboCount, GL_UNSIGNED_INT, 0);
    }
}
buffersWrapper::~buffersWrapper()
{
    bindVao(); 
    if(eboCount != 0)
    {
        glDeleteBuffers(1, &EBO);
    }
    for(int i = 0; i < VBOs.size(); i++)
    {
        glDeleteBuffers(1, &VBOs[i]);
    }
    glDeleteVertexArrays(1, &VAO);
    return;
}