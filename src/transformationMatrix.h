#pragma once

class matrix4x4f
{
protected:
    float mat[16];
public:
    matrix4x4f();
    matrix4x4f(float* mat);
    matrix4x4f operator*(const matrix4x4f& other);
    const float* getMat()
    {
        return mat;
    }
};
class rotMatrix : public matrix4x4f
{
public:
    rotMatrix(float x, float y, float z, float angle);
};
class translMatrix : public matrix4x4f
{
public:
    translMatrix(float x, float y, float z);
};
class projMatrix : public matrix4x4f
{
public:
    projMatrix(float fov, float aspect, float near, float far);
};
class idMatrix : public matrix4x4f
{
public:
    idMatrix();
};


