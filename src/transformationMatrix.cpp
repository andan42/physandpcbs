#include "transformationMatrix.h"

#include <cmath>

matrix4x4f::matrix4x4f()
{
        for (int i = 0; i < 16; i++)
        {
            mat[i] = 0.0f;
        }
}
matrix4x4f::matrix4x4f(float* mat)
{
        for (int i = 0; i < 16; i++)
        {
            this->mat[i] = mat[i];
        }
}
matrix4x4f matrix4x4f::operator*(const matrix4x4f& other)
{
    matrix4x4f result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.mat[i * 4 + j] += mat[i * 4 + k] * other.mat[k * 4 + j];
            }
        }
    }
    return result;
}

rotMatrix::rotMatrix(float x, float y, float z, float angle)
{
    // normalize vector
    float length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
    // calculate rotation matrix
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - c;
    mat[0] = t * x * x + c;
    mat[1] = t * x * y - s * z;
    mat[2] = t * x * z + s * y;
    mat[3] = 0.0f;
    mat[4] = t * x * y + s * z;
    mat[5] = t * y * y + c;
    mat[6] = t * y * z - s * x;
    mat[7] = 0.0f;
    mat[8] = t * x * z - s * y;
    mat[9] = t * y * z + s * x;
    mat[10] = t * z * z + c;
    mat[11] = 0.0f;
    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = 0.0f;
    mat[15] = 1.0f;
}
translMatrix::translMatrix(float x, float y, float z)
{
    mat[0] = 1.0f;
    mat[1] = 0.0f;
    mat[2] = 0.0f;
    mat[3] = 0.0f;
    mat[4] = 0.0f;
    mat[5] = 1.0f;
    mat[6] = 0.0f;
    mat[7] = 0.0f;
    mat[8] = 0.0f;
    mat[9] = 0.0f;
    mat[10] = 1.0f;
    mat[11] = 0.0f;
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
    mat[15] = 1.0f;
}
projMatrix::projMatrix(float fov, float aspect, float near, float far)
{
    float f = 1.0f / tan(fov / 2.0f);
    mat[0] = f / aspect;
    mat[1] = 0.0f;
    mat[2] = 0.0f;
    mat[3] = 0.0f;
    mat[4] = 0.0f;
    mat[5] = f;
    mat[6] = 0.0f;
    mat[7] = 0.0f;
    mat[8] = 0.0f;
    mat[9] = 0.0f;
    mat[10] = (far + near) / (near - far);
    mat[11] = -1.0f;
    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = (2.0f * far * near) / (near - far);
    mat[15] = 0.0f;
}



