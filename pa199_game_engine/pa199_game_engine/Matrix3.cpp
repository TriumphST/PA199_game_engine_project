#include "Matrix3.h"
#include <stdio.h>

Matrix3::Matrix3()
{
}

Matrix3::Matrix3(float init[3][3])
{
    if (init == NULL)
        return;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            core[row][col] = init[row][col];
        }
    }
}

bool Matrix3::GetIsSingular()
{
    return isSingular;
}

bool Matrix3::operator==(const Matrix3& other)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (other.core[row][col] != core[row][col]) {
                return false;
            }
        }
    }
    return true;
}

float *Matrix3::operator[](const int& row)
{
    return core[row];
}

Matrix3 Matrix3::operator*(const Matrix3& other)
{
    Matrix3 results = Matrix3();

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            for (int u = 0; u < 3; u++)
                results[i][j] += core[i][u] * other.core[u][j];
        }
    return results;
}

Matrix3 Matrix3::operator+(const Matrix3& other)
{
    Matrix3 result = Matrix3();
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            result[row][col] = core[row][col] + other.core[row][col];
        }
    }
    return result;
}

Matrix3 Matrix3::operator-(const Matrix3& other)
{
    Matrix3 result = Matrix3();
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            result[row][col] = core[row][col] - other.core[row][col];
        }
    }
    return result;
}

Matrix3 Matrix3::transpose()
{
    Matrix3 tM = Matrix3();
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            tM[col][row] = core[row][col];
        }
    }
    return tM;
}

Matrix3* Matrix3::inverse()
{
    //finding determinant
    float determinant = 0;
    for (int i = 0; i < 3; i++)
        determinant = determinant + (core[0][i] * (core[1][(i + 1) % 3] * core[2][(i + 2) % 3] - core[1][(i + 2) % 3] * core[2][(i + 1) % 3]));

    if (determinant == 0) {
        return NULL;
    }

    Matrix3 inv = Matrix3();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            inv[i][j] = ((core[(j + 1) % 3][(i + 1) % 3] * core[(j + 2) % 3][(i + 2) % 3]) - (core[(j + 1) % 3][(i + 2) % 3] * core[(j + 2) % 3][(i + 1) % 3])) / determinant;
    }
    return &inv;
}
