#include "renderingWrapper.h"

renderingWrapper::renderingWrapper(): windowW("Hello World")
{
    shaderWrapper vertexShader("shader/test.vert", GL_VERTEX_SHADER);
    shaderWrapper fragmentShader("shader/test.frag", GL_FRAGMENT_SHADER);
    shaderProgramWrapper _shaderProgram(vertexShader, fragmentShader);
    //move shader program to member variable
    shaderProgramW = std::move(_shaderProgram);

    //buffers
    dummyTriBuffer.addBuffer(0, 4, triangleVerts, sizeof(triangleVerts));
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
    dummyResizingBuffer.setEbo(cubeIndices, sizeof(cubeIndices));
    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

bool renderingWrapper::loop()
{
    processInput(windowW.window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgramW.use();

    float shine = (2 - std::sin(glfwGetTime() * 2 * 3.14)) / 3.0;
    glUniform1f(0, shine);

    glUniformMatrix4fv(1, 1, GL_FALSE, 
    (
    rotMatrix(0.0f, 1.0f, 0.0f, glfwGetTime() * 3.14)
    *
    rotMatrix(1.0f, 0.0f, 0.0f, glfwGetTime() * 3.14 / 10.0)
    )
    
    .getMat());

    //dummyTriBuffer.draw();
    //dummyRectBuffer.draw();
    //dummyCubeBuffer.draw();
    dummyResizingBuffer.draw();

    glfwSwapBuffers(windowW.window);
    glfwPollEvents();

    return !glfwWindowShouldClose(windowW.window);
}

renderingWrapper::~renderingWrapper()
{
}