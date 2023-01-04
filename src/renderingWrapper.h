#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "windowWrapper.h"
#include "shaderProgramWrapper.h"
#include "buffersWrapper.h"

#include "transformationMatrix.h"
class renderingWrapper
{
private:
    windowWrapper windowW;
    
    shaderProgramWrapper shaderProgramW;
    

    float triangleVerts[12]= {
         0.0f, -0.5f - 0.5f, -0.5f, 1.0f, // left  
         0.0f, -0.5f - 0.5f, 0.5f,  1.0f,   // right 
         0.0f,  0.5f - 0.5f, 0.0f,  1.0f  // top
    };
    float triangleNormals[12] = {
        0.0f, 0.0f, 1.0f, 0.0, // left  
        0.0f, 0.0f, 1.0f, 0.0, // right 
        0.0f, 0.0f, 1.0f,  0.0  // top   
    };
    float triangleColors[9] = {
        1.0f, 0.0f, 0.0f, // left  r 
        0.0f, 1.0f, 0.0f, // right g
        0.0f, 0.0f, 1.0f  // top   b
    };
    unsigned int triangleIndices[3] = {
        0, 1, 2
    };
    float rectangleVerts[16] = {
        -0.5f, -0.5f, 0.0f, 1.0f, // left  
        0.5f, -0.5f, 0.0f, 1.0f,   // right 
        0.5f,  0.5f, 0.0f, 1.0f,  // top left
        -0.5f,  0.5f, 0.0f, 1.0f // top right
    };
    float rectangleNormals[16] = {
        0.0f, 0.0f, 1.0f, 0.0, // left  
        0.0f, 0.0f, 1.0f, 0.0, // right 
        0.0f, 0.0f, 1.0f,  0.0,  // top left
        0.0f, 0.0f, 1.0f,  0.0 // top right
    };
    float rectangleColors[12] = {
        1.0f, 0.0f, 0.0f, // left  r 
        0.0f, 1.0f, 0.0f, // right g
        0.0f, 0.0f, 1.0f,  // top left b
        1.0f, 1.0f, 1.0f // top right w
    };
    unsigned int rectangleIndices[6] = {
        0, 1, 2, 0, 2, 3
    };
    float cubeVerts[32] = {
        // front
        -0.5f, -0.5f, 0.5f, 1.0f, // bottom left  
        0.5f, -0.5f, 0.5f, 1.0f,   // bottom right 
        0.5f,  0.5f, 0.5f, 1.0f,  // top right
        -0.5f,  0.5f, 0.5f, 1.0f, // top left
        // back
        -0.5f, -0.5f, -0.5f, 1.0f, // bottom left  
        0.5f, -0.5f, -0.5f, 1.0f,   // bottom right 
        0.5f,  0.5f, -0.5f, 1.0f,  // top right
        -0.5f,  0.5f, -0.5f, 1.0f // top left
    };
    float cubeNormals[32] = {
        // front
        0.0f, 0.0f, 1.0f, 0.0, // left  
        0.0f, 0.0f, 1.0f, 0.0, // right 
        0.0f, 0.0f, 1.0f,  0.0,  // top left
        0.0f, 0.0f, 1.0f,  0.0, // top right
        // back
        0.0f, 0.0f, -1.0f, 0.0, // left  
        0.0f, 0.0f, -1.0f, 0.0, // right 
        0.0f, 0.0f, -1.0f,  0.0,  // top left
        0.0f, 0.0f, -1.0f,  0.0 // top right
    };
    float cubeColors[24] = {
        // front
        1.0f, 0.0f, 0.0f, // bottom left  r 
        0.0f, 1.0f, 0.0f, // bottom right g
        0.0f, 0.0f, 1.0f,  // top left b
        1.0f, 1.0f, 1.0f, // top right w
        // back
        1.0f, 0.0f, 0.0f, // left  r 
        0.0f, 1.0f, 0.0f, // right g
        0.0f, 0.0f, 1.0f,  // top left b
        0.0f, 0.0f, 0.0f // top right bl
    };
    float triangleColorsGrey[9] = {
        0.5f, 0.5f, 0.5f, // left  gry
        0.5f, 0.5f, 0.5f, // right gry
        0.5f, 0.5f, 0.5f  // top   gry
    };

    //swap like this:
    // a, b, c, d, e, f -> c, b, a, f, e, d
    unsigned int cubeIndices[36] = {
        // front
        //0, 1, 2, 2, 3, 0,
        //swap front
        2, 1, 0, 0, 3, 2,
        // back
        4, 5, 6, 6, 7, 4,
        //swap back
        //6, 5, 4, 4, 7, 6,
        //left
        0, 4, 7, 7, 3, 0,
        //swap left
        //7, 4, 0, 0, 3, 7,
        //right
        //1, 5, 6, 6, 2, 1,
        //swap right
        6, 5, 1, 1, 2, 6,
        //top
        //2, 6, 7, 7, 3, 2,
        //swap top
        7, 6, 2, 2, 3, 7,
        //bottom
        //0, 4, 5, 5, 1, 0
        //swap bottom
        5, 4, 0, 0, 1, 5
        // <3333       
        // good swapping chief! 

    };
        

    buffersWrapper dummyTriBuffer;
    buffersWrapper dummyRectBuffer;
    buffersWrapper dummyCubeBuffer;

    buffersWrapper dummyResizingBuffer;
public:
    renderingWrapper();
    //void setup();
    bool loop();
    //void cleanup();
    ~renderingWrapper();
};