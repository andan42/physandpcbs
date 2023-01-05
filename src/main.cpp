#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

#include "renderingWrapper.h"



int main()
{
    int eFieldSize = 100;
    //make a 100x100 contigous array of floats
    float* eField = new float[eFieldSize * eFieldSize];
    //fill it with zeros
    for (int i = 0; i < 100 * 100; i++)
    {
        eField[i] = 0.0f;
    }
    //make a square of 1.0f in the middle of the grid

    
    renderingWrapper bigR;
    bigR.field = eField;
    bigR.fieldSideLength = &eFieldSize;
    bigR.textureUpdate();
    int c = 40;
    while (bigR.loop())
    {

        bigR.textureUpdate();
        c++;
        //make a gradient of -1.0 to 1.0 that slides across the screen and loops back
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                eField[i * 100 + j] = (float)((i + c) % 100 - 50) / 50.0f;
            }
        }
    }
    return 0;
}

