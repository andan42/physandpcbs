#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class buffersWrapper //todo: add and set buffer by location name function 
{
public:
    GLuint VAO; 
    static GLuint boundVAO; //the VAO that is currently bound <3 <3 <3, this is so that we dont have to bind the VAO every time we want to bind a buffer, we can just check if the VAO is already bound, and if it is, then we dont need to bind it again, and if it isnt, then we need to bind it, and then we can bind the buffer <3 <3 <3
    GLuint EBO;
    int eboCount = 0; //the number of indices in the EBO <3 <3 <3
    int vertexCount = 0; //the number of vertices each VBO must have <3 <3 <3
    std::vector<GLuint> VBOs;
    
    std::vector<GLuint> bufferTypes; 
    std::vector<GLuint> bufferUsages; //save the usage of each buffer so that we can update the buffer later
    
    std::vector<int> vertexCounts; //the number of vertices in each VBO <3 <3 <3

    std::vector<bool> normalizes; //normalize corresponding to each buffer. this is needed to check if the buffer is normalized or not
    std::vector<GLuint> attrLocations; //attr location corresponding to each buffer. this is because the buffers arent added in the same order as the attributes in the shader
    std::vector<GLuint> attrSizes; //attr size corresponding to each buffer. this is needed to calculate vertex count per buffer
    buffersWrapper();
    void bindVao(); //TODO make RAII based VAO binder, that binds vao when created and binds old vao when destroyed <3 <3 <3
    //<3 this assumes unsigned int type data <3 <3 <3 dataSize is in bytes
    //sets the EBO for the VAO, if it already has an EBO, it will overwrite it, otherwise it will create a new one, and set it as the EBO for the VAO, and set hasEbo to true, so that it knows that it has an EBO, and that it should delete it when the buffersWrapper object is deleted, and that it should overwrite it if this function is called again, and that it should bind it when the VAO is bound, actually the vao binds it automatically, so we dont need to bind it here, but we do need to set it as the EBO for the VAO, so that it knows that it should use it when it is bound, and that it should delete it when the VAO is deleted <3 <3 <3 
     //make sure to bind the previous vao after calling this function <3 <3 <3 <3
    void setEbo(GLuint* data, int dataSize /*in bytes*/, GLenum usage = GL_STATIC_DRAW);
    // <3 this assumes float type data .. dataSize is in bytes
    //adds a buffer to the VAO, and sets the attribute location for the buffer, and sets the buffer type, and sets the buffer usage, and sets the normalize flag, and sets the stride, and sets the offset, and sets the buffer data, and sets the attribute pointer, and enables the attribute array, and saves the buffer type, buffer usage, and attribute location, so that we can update the buffer later, and so that we can bind the buffer later, and so that we can set the attribute pointer later, and so that we can enable the attribute array later, and so that we can delete the buffer later <3 <3 <3
     //make sure to bind the previous vao after calling this function <3 <3 <3 <3
    void addBuffer(int attrLocation, int attrSize, GLfloat* data, int dataSize /*in bytes*/, GLenum bufferType = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW, bool normalize = false);
    // <3 this assumes float type data
    //dataSize is in bytes
    //updates the buffer with the given attribute location setting the buffer data <3 <3 <3
     //make sure to bind the previous vao after calling this function <3 <3 <3 <3
    void updateBuffer(int attrLocation, GLfloat* data, int dataSize /*in bytes*/);
    //resets the vertex count to 0
    //after resetting vertex count to 0, each buffer needs to have new data set to it, otherwise validation will fail, and the buffers will not be drawn <3 <3 <3
    void resetBuffersSize();
    //checks if each vertexCounts is equal to vertexCount. this is needed before drawing the buffers, because if the vertexCounts are not equal to vertexCount, then the buffers are not valid, and we should not draw them <3 <3 <3
    bool validateBufferSizes();
    //the destructor binds its VAO, and deletes its EBO, and deletes its VBOs, and deletes its VAO <3 <3 <3
    //make sure to bind the previous vao after calling this function <3 <3 <3 <3
    void draw();
    
    ~buffersWrapper();
};
