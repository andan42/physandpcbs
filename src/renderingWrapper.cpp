#include "renderingWrapper.h"

renderingWrapper::renderingWrapper(): windowW("Hello World")
{
    //shaders
    {shaderWrapper vertexShader("shader/test.vert", GL_VERTEX_SHADER);
    shaderWrapper fragmentShader("shader/test.frag", GL_FRAGMENT_SHADER);
    shaderProgramWrapper _shaderProgram(std::move(vertexShader), std::move(fragmentShader));
    shaderWrapper vertexShaderTex("shader/texturetest.vert", GL_VERTEX_SHADER);
    shaderWrapper fragmentShaderTex("shader/texturetest.frag", GL_FRAGMENT_SHADER);
    shaderProgramWrapper _shaderProgramTex(std::move(vertexShaderTex), std::move(fragmentShaderTex));
    //move shader program to member variable
    shaderProgramW = std::move(_shaderProgram);
    shaderProgramTexW = std::move(_shaderProgramTex);}
    //buffers
    {/*dummyTriBuffer.addBuffer(0, 4, triangleVerts, sizeof(triangleVerts));
    dummyTriBuffer.addBuffer(1, 4, triangleNormals, sizeof(triangleNormals));
    dummyTriBuffer.addBuffer(2, 3, triangleColors, sizeof(triangleColors));
    //dummyTriBuffer.setEbo(triangleIndices, sizeof(triangleIndices));

    dummyRectBuffer.addBuffer(0, 4, rectangleVerts, sizeof(rectangleVerts));
    dummyRectBuffer.addBuffer(1, 4, rectangleNormals, sizeof(rectangleNormals));
    dummyRectBuffer.addBuffer(2, 3, rectangleColors, sizeof(rectangleColors));
    dummyRectBuffer.setEbo(rectangleIndices, sizeof(rectangleIndices));

    dummyCubeBuffer.addBuffer(0, 4, cubeVerts, sizeof(cubeVerts));
    dummyCubeBuffer.addBuffer(1, 4, cubeNormals, sizeof(cubeNormals));
    dummyCubeBuffer.addBuffer(2, 3, cubeColors, sizeof(cubeColors));
    dummyCubeBuffer.setEbo(cubeIndices, sizeof(cubeIndices));

    //set resizing buffer to cube   
    dummyResizingBuffer.addBuffer(0, 4, cubeVerts, sizeof(cubeVerts));
    dummyResizingBuffer.addBuffer(1, 4, cubeNormals, sizeof(cubeNormals));
    dummyResizingBuffer.addBuffer(2, 3, cubeColors, sizeof(cubeColors));
    dummyResizingBuffer.setEbo(cubeIndices, sizeof(cubeIndices));*/
    
    //buffer with texture, (texture coords buffer, but without colors or normals)
    dummyTexturedbuffer.addBuffer(0, 4, screenRectVerts, sizeof(rectangleVerts));
    dummyTexturedbuffer.addBuffer(1, 2, rectangleTexCoords, sizeof(rectangleTexCoords));
    dummyTexturedbuffer.setEbo(rectangleIndices, sizeof(rectangleIndices));}
    //perfect <3 <3 <3

    shaderProgramTexW.use();
    //rotation uniform
    glUniformMatrix4fv(0, 1, GL_FALSE, idMatrix().getMat());


    //shaderProgramW.use(); this is already in use

    //generate texture texture1 (field)
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    texture1location = 1; //this is layout location = 0
    glUniform1i(texture1location, 0); //set texture1 to texture unit 0

    //set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //perfect <3

    
    //generate texture texture2 (mask)
    glActiveTexture(GL_TEXTURE1);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    texture2location = 2; //this is layout location = 1
    glUniform1i(texture2location, 1); //set texture2 to texture unit 1

    //set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //perfect <3


    //misc 
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}

void renderingWrapper::textureUpdate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, *fieldSideLength, *fieldSideLength, 0, GL_RED, GL_FLOAT, field); //this is the data field of this class (the eField) <33333

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, *fieldSideLength, *fieldSideLength, 0, GL_RED, GL_FLOAT, mask); //this is the data field of this class (the mask <3) <33333

}

bool renderingWrapper::loop()
{
    //process input and clear screen
    processInput(windowW.window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*
    //draw tri buffer
    shaderProgramW.use();

    float shine = (2 - std::sin(glfwGetTime() * 2 * 3.14)) / 3.0;
    glUniform1f(0, shine);

    glUniformMatrix4fv(1, 1, GL_FALSE, (rotMatrix(0.0f, 1.0f, 0.0f, glfwGetTime() * 3.14)*rotMatrix(1.0f, 0.0f, 0.0f, glfwGetTime() * 3.14 / 10.0)).getMat());
    
    dummyTriBuffer.draw();
    //end draw tri buffer
*/


    //draw rect tex buffer
    shaderProgramTexW.use();

    //glUniformMatrix4fv(0, 1, GL_FALSE, (rotMatrix(0.0f, 1.0f, 0.0f, glfwGetTime() * 3.14)*rotMatrix(1.0f, 0.0f, 0.0f, glfwGetTime() * 3.14 / 10.0)).getMat());
    
    dummyTexturedbuffer.draw();
    //end draw rect tex buffer

    //swap buffers and poll events
    glfwSwapBuffers(windowW.window);
    glfwPollEvents();
    return !glfwWindowShouldClose(windowW.window);
}

renderingWrapper::~renderingWrapper()
{
}