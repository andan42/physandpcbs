#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>

//include stuff to sleep
#include <chrono>
#include <thread>

#include "renderingWrapper.h"
//write laplacian pseudocode
//write laplacian function
//write laplacian function in c++
float fieldSamplerFixedBoundary(int x, int y, float* field, int fieldSideLength)
{
    if(x < 0 || x >= fieldSideLength || y < 0 || y >= fieldSideLength)
    {
        return 0.0f;
    }
    else
    {
        return field[x + y * fieldSideLength];
    }
};
float fieldSamplerReflectedBoundary(int x, int y, float* field, int fieldSideLength)
{
    if(x < 0)
    {
        x = -x;
    }
    else if(x >= fieldSideLength)
    {
        x = 2 * fieldSideLength - x - 1;
    }
    if(y < 0)
    {
        y = -y;
    }
    else if(y >= fieldSideLength)
    {
        y = 2 * fieldSideLength - y - 1;
    }
    return field[x + y * fieldSideLength];
};
float fieldSamplerIgnoreBoundary(int x, int y, float* field, int fieldSideLength)
{
    if(x < 0 || x >= fieldSideLength || y < 0 || y >= fieldSideLength)
    {
        //throw
        throw "fieldSamplerIgnoreBoundary: out of bounds"; //remove this later, if we see code doesnt throw in debug <3
        return 0.0f;
    }
    else
    {
        return field[x + y * fieldSideLength];
    }
};

/*
float laplacianPoint(int x, int y, float* field, int fieldSideLength, float samp(int, int, float*, int))
{   
    int xm, xp, ym, yp;
    if(x == 0)
    {   
        xm = x;
        xp = x + 1;
    }
    else if(x == fieldSideLength - 1)
    {
        xm = x - 1;
        xp = x;
    }
    else
    {
        xm = x - 1;
        xp = x + 1;
    }
    if(y == 0)
    {
        ym = y;
        yp = y + 1;
    }
    else if(y == fieldSideLength - 1)
    {
        ym = y - 1;
        yp = y;
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
*/
#define LPL_CONST 0.001
float laplacianPoint(int x, int y, float* field, int fieldSideLength, float samp(int, int, float*, int))
{
    float laplacian = 0.0f;
    laplacian += samp(x - 1, y - 1, field, fieldSideLength);
    laplacian += samp(x - 1, y + 0, field, fieldSideLength);
    laplacian += samp(x - 1, y + 1, field, fieldSideLength);
    laplacian += samp(x + 0, y - 1, field, fieldSideLength);
    laplacian += samp(x + 0, y + 1, field, fieldSideLength);
    laplacian += samp(x + 1, y - 1, field, fieldSideLength);
    laplacian += samp(x + 1, y + 0, field, fieldSideLength);
    laplacian += samp(x + 1, y + 1, field, fieldSideLength);
    laplacian -= 8.0f * samp(x, y, field, fieldSideLength);
    return laplacian;
}
void laplacianField(float* field, float*laplacian, int fieldSideLength, float samp(int, int, float*, int))
{
    for (int y = 0; y < fieldSideLength; y++)
    {
        for (int x = 0; x < fieldSideLength; x++)
        {
            laplacian[x + y * fieldSideLength] = LPL_CONST * laplacianPoint(x, y, field, fieldSideLength, samp);
        }
    }
}

void laplacianFieldFixedBounds(float* field, float*laplacian, int fieldSideLength)
{
    //loop over all points in the field except boundary points
    for (int y = 1; y < fieldSideLength - 1; y++)
    {
        for (int x = 1; x < fieldSideLength - 1; x++)
        {
            laplacian[x + y * fieldSideLength] = LPL_CONST * laplacianPoint(x, y, field, fieldSideLength, fieldSamplerFixedBoundary);
        }
    }
}


void laplacianFieldFreeBounds(float* field, float*laplacian, int fieldSideLength)
{
    //loop over all points in the field except boundary points
    for (int y = 1; y < fieldSideLength - 1; y++)
    {
        for (int x = 1; x < fieldSideLength - 1; x++)
        {
            laplacian[x + y * fieldSideLength] = LPL_CONST * laplacianPoint(x, y, field, fieldSideLength, fieldSamplerIgnoreBoundary);
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
void halfField(float* field, int fieldSideLength)
{
    for (int i = 0; i < fieldSideLength * fieldSideLength; i++)
    {
        field[i] *= 0.5f;
    }
}
void copyField(float* targetF, float* sourceF, int fieldSideLength)
{
    for (int i = 0; i < fieldSideLength * fieldSideLength; i++)
    {
        targetF[i] = sourceF[i];
    }
}
void attenuateField(float* field, float* eFieldFirstDerivative, float attenuation, int fieldSideLength)
{
    for (int i = 0; i < fieldSideLength * fieldSideLength; i++)
    {
        field[i] *= attenuation;
        eFieldFirstDerivative[i] *= attenuation;
    }
}
void attenuateFieldBorder(float* field, float* eFieldFirstDerivative, float attenuation, int borderThickness, int fieldSideLength)
{
    for (int y = 0; y < fieldSideLength; y++)
    {
        for (int x = 0; x < fieldSideLength; x++)
        {
            if (x < borderThickness || x > fieldSideLength - borderThickness || y < borderThickness || y > fieldSideLength - borderThickness)
            {
                field[x + y * fieldSideLength] *= attenuation;
                eFieldFirstDerivative[x + y * fieldSideLength] *= attenuation;
            }
        }
    }
}
//make a square wave function with same period as sin
float squareWave(float x)
{
    float y = sin(x);
    if (y > 0.0f)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

int main()
{
    int eFieldSize = 300;
    //make a 100x100 contigous array of floats
    float* eField = new float[eFieldSize * eFieldSize];
    float* eFieldFirstDerivative = new float[eFieldSize * eFieldSize];
    float* eFieldLaplacian = new float[eFieldSize * eFieldSize];

    float* solidObjectMask = new float[eFieldSize * eFieldSize];

    for (int i = 0; i < eFieldSize * eFieldSize; i++)
    {
        //fill it with zeros
        eField[i] = 0.0f;
        eFieldFirstDerivative[i] = 0.0f;
        //fill mask with ones
        solidObjectMask[i] = 1.0f;
    }
    //give a weird dipole initial condition on the top right
    
    //eField[(eFieldSize - 51) + (eFieldSize - 50) * eFieldSize] = 10.0f;
    //eField[(eFieldSize - 51) + (eFieldSize - 50) * eFieldSize] = -10.0f;


    //make a radius 5 circle around the point above
    for (int y = eFieldSize - 55; y < eFieldSize - 45; y++)
    {
        for (int x = eFieldSize - 55; x < eFieldSize - 45; x++)
        {
            if (sqrt((x - (eFieldSize - 50)) * (x - (eFieldSize - 50)) + (y - (eFieldSize - 50)) * (y - (eFieldSize - 50))) < 5.0f)
            {
                //normalized distance from center
                float distn = sqrt((x - (eFieldSize - 50)) * (x - (eFieldSize - 50)) + (y - (eFieldSize - 50)) * (y - (eFieldSize - 50)))/5.0;
                eField[x + y * eFieldSize] = 10.0f * (cos(distn * 3.14159f / 2.0f) * cos(distn * 3.14159f / 2.0f));
            }
        }
    }
    //

    renderingWrapper bigR;
    bigR.field = eField;
    bigR.mask = solidObjectMask;
    bigR.fieldSideLength = &eFieldSize;
    bigR.textureUpdate();
    //sleep 2 secs
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    float timer = 0.0f;
    while (bigR.loop())
    {
        //do stuff
        laplacianField(eField, eFieldLaplacian, eFieldSize, fieldSamplerFixedBoundary);
        addField(eFieldFirstDerivative, eFieldLaplacian, eFieldSize);

        addField(eField, eFieldFirstDerivative, eFieldSize);

        float freq = 0.5f;
        float period = 1.0f / freq;
        //oscillator sources
        float wavegen1 = sin(timer * freq * 2 * 3.14159265359f) + sin(timer * freq/2 * 2 * 3.14159265359f);

        //is pulisng while timer < period
        bool isPulsing = timer < period * 2.0f;
        float pulsegen1 = (isPulsing ? 1.0f : 0.0f) * wavegen1 * 1;

        //downwards pointin reflecting parabola
        for(int i = -25; i < 25; i++)
        {
            //boundary conditions to reflect the wave
            eField[50 + i + (eFieldSize - 25 - i*i / 50) * eFieldSize] = 0.0f;
            eField[50 + i + (eFieldSize - 24 - i*i / 50) * eFieldSize] = 0.0f;
            eField[50 + i + (eFieldSize - 23 - i*i / 50) * eFieldSize] = 0.0f;
            //mask to display objects in black
            solidObjectMask[50 + i + (eFieldSize - 25 - i*i / 50) * eFieldSize] = 0.0f;
            solidObjectMask[50 + i + (eFieldSize - 24 - i*i / 50) * eFieldSize] = 0.0f;
            solidObjectMask[50 + i + (eFieldSize - 23 - i*i / 50) * eFieldSize] = 0.0f;
        }

        
        //point source at 50, fieldsize - 50
        if(isPulsing)
        {
            //eField[50 + (eFieldSize - 51) * eFieldSize] = pulsegen1 * 10.0f;
            //eField[50 + (eFieldSize - 50) * eFieldSize] = pulsegen1 * -10.0f;
        }
        //point source at 50, fieldsize - 50 + 25/2 -1
        if(isPulsing)
        {
            eField[50 + (eFieldSize - 51 + 25/2) * eFieldSize] = pulsegen1 * 10.0f;
            eField[50 + (eFieldSize - 50 + 25/2) * eFieldSize] = pulsegen1 * -10.0f;
            solidObjectMask[50 + (eFieldSize - 51 + 25/2 ) * eFieldSize] = 0.0f;
            solidObjectMask[50 + (eFieldSize - 50 + 25/2 ) * eFieldSize] = 0.0f;
        }

        //diagonal mirror at 50, 50
        for (int i = -50; i < 50; i++)
        {
            //boundary conditions to reflect the wave
            float coeff = 0.0f; //this was 0.5 for some reason but i changed it back? idk maybe copilot did it? idk 
            //how does this coeff affect the wave? i dont know

            eField[50 + i + (50 - i) * eFieldSize] = eField[50 + i + (50 - i) * eFieldSize] * coeff;
            eField[50 + i + (50 - i) * eFieldSize] = eField[50 + i + (50 - i) * eFieldSize] * coeff;
            eField[50 + i + (50 - i) * eFieldSize] = eField[50 + i + (50 - i) * eFieldSize] * coeff;
            //mask to display objects in black
            solidObjectMask[50 + i + (50 - i) * eFieldSize] = 0.0f;
            solidObjectMask[50 + i + (50 - i) * eFieldSize] = 0.0f;
            solidObjectMask[50 + i + (50 - i) * eFieldSize] = 0.0f;

        }

        //leftwards pointing parabola at efieldsize -50 , 50

        for (int i = -25; i < 25; i++)
        {
            //boundary conditions to reflect the wave
            eField[(eFieldSize - 25 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
            eField[(eFieldSize - 24 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
            eField[(eFieldSize - 23 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
            //mask to display objects in black
            solidObjectMask[(eFieldSize - 25 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
            solidObjectMask[(eFieldSize - 24 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
            solidObjectMask[(eFieldSize - 23 - i*i / 100) + (50 + i) * eFieldSize] = 0.0f;
        }

        attenuateFieldBorder(eField, eFieldFirstDerivative, 0.99f, 5, eFieldSize);

        bigR.textureUpdate();
        timer += 0.01f;
        //sleep 1/60th of a second //todo //how to sleep in cpp cross platform?
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));

    }
    return 0;
}

