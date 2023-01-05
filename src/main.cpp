#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "renderingWrapper.h"



int main()
{
    //make a 100x100 contigous array of floats
    float* eField = new float[100 * 100];
    //fill it with zeros
    for (int i = 0; i < 100 * 100; i++)
    {
        eField[i] = 0.0f;
    }
    //make a square of 1.0f in the middle of the grid
    for (int i = 40; i < 60; i++)
    {
        for (int j = 40; j < 60; j++)
        {
            eField[i * 100 + j] = 1.0f;
        }
    }
    
    renderingWrapper bigR;
    bigR.data = eField;
    bigR.textureUpdate();
    while (bigR.loop())
    {
    }
    return 0;
}

