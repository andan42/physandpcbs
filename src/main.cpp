#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

//include stuff to sleep
#include <chrono>
#include <thread>

#include "renderingWrapper.h"

float laplacianPoint(int x, int y, float* field, int fieldSideLength)
{
    int xm, xp, ym, yp;
    if(x == 0)
    {   
        xm = fieldSideLength - 1;
        xp = x + 1;
    }
    else if(x == fieldSideLength - 1)
    {
        xm = x - 1;
        xp = 0;
    }
    else
    {
        xm = x - 1;
        xp = x + 1;
    }
    if(y == 0)
    {
        ym = fieldSideLength - 1;
        yp = y + 1;
    }
    else if(y == fieldSideLength - 1)
    {
        ym = y - 1;
        yp = 0;
    }
    else
    {
        ym = y - 1;
        yp = y + 1;
    }
    float laplacian = 0.0f;
    //laplacian += adjacent or -= ??  do 8 cells or 4 cells? 
    laplacian += field[(xm + 0) + (y + 0) * fieldSideLength];
    laplacian += field[(x + 0) + (ym + 0) * fieldSideLength];
    laplacian += field[(xp + 0) + (y + 0) * fieldSideLength];
    laplacian += field[(x + 0) + (yp + 0) * fieldSideLength];
    //do diagonals too
    laplacian += field[(xm + 0) + (ym + 0) * fieldSideLength];
    laplacian += field[(xp + 0) + (ym + 0) * fieldSideLength];
    laplacian += field[(xm + 0) + (yp + 0) * fieldSideLength];
    laplacian += field[(xp + 0) + (yp + 0) * fieldSideLength];
    //thanks <3

    laplacian -= 8.0f * field[(x + 0) + (y + 0) * fieldSideLength];
    return laplacian;
}
void laplacianField(float* field, float*laplacian, int fieldSideLength)
{
    for (int y = 0; y < fieldSideLength; y++)
    {
        for (int x = 0; x < fieldSideLength; x++)
        {
            laplacian[x + y * fieldSideLength] = 0.001 * laplacianPoint(x, y, field, fieldSideLength);
        }
    }
}
void addField(float* field, float* add, int fieldSideLength)
{
    for (int i = 0; i < fieldSideLength * fieldSideLength; i++)
    {
        field[i] += add[i];
    }
}
void subtractField(float* field, float* subtract, int fieldSideLength)
{
    for (int i = 0; i < fieldSideLength * fieldSideLength; i++)
    {
        field[i] -= subtract[i];
    }
}
int main()
{
    int eFieldSize = 100;
    //make a 100x100 contigous array of floats
    float* eField = new float[eFieldSize * eFieldSize];
    float* eFieldFirstDerivative = new float[eFieldSize * eFieldSize];
    float* eFieldLaplacian = new float[eFieldSize * eFieldSize];
    //fill it with zeros
    for (int i = 0; i < 100 * 100; i++)
    {
        eField[i] = 0.0f;
        eFieldFirstDerivative[i] = 0.0f;
    }

    renderingWrapper bigR;
    bigR.field = eField;
    bigR.fieldSideLength = &eFieldSize;
    bigR.textureUpdate();
    //sleep 2 secs
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    float timer = 0.0f;
    while (bigR.loop())
    {
        //do stuff
        laplacianField(eField, eFieldLaplacian, eFieldSize);
        addField(eFieldFirstDerivative, eFieldLaplacian, eFieldSize);
        addField(eField, eFieldFirstDerivative, eFieldSize);
        //oscillate 10x10 pixel in middle
        float wavegen = sin(timer);
        for(int i = -5; i < 5; i++)
        {
            for(int j = -5; j < 5; j++)
            {
                eField[(eFieldSize / 2 + i) + (eFieldSize / 2 + j) * eFieldSize] = wavegen;
            }
        }

        bigR.textureUpdate();
        timer += 0.01f;
        //sleep 1/60th of a second //todo //how to sleep in cpp cross platform?
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

    }
    return 0;
}

