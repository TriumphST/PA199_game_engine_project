#include "Matrix4.h"
#include <stdio.h>

Matrix4::Matrix4()
{
}

Matrix4::Matrix4(float init[4][4])
{
    if (init == NULL)
        return;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            core[row][col] = init[row][col];
        }
    }
}

Matrix4::Matrix4(float initNumber)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (row == col)
                core[row][col] = initNumber;
            else
                core[row][col] = 0.0f;
        }
    }
}

bool Matrix4::GetIsSingular()
{
    return isSingular;
}

bool Matrix4::operator==(const Matrix4& other)
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (other.core[row][col] != core[row][col]) {
                return false;
            }
        }
    }
    return true;
}

float* Matrix4::operator[](const int& row)
{
    return core[row];
}

Matrix4 Matrix4::operator*(const Matrix4& other)
{
    Matrix4 results = Matrix4();

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            for (int u = 0; u < 4; u++)
                results[i][j] += core[i][u] * other.core[u][j];
        }
    return results;
}

Matrix4 Matrix4::operator+(const Matrix4& other)
{
    Matrix4 result = Matrix4();
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            result[row][col] = core[row][col] + other.core[row][col];
        }
    }
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& other)
{
    Matrix4 result = Matrix4();
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result[row][col] = core[row][col] - other.core[row][col];
        }
    }
    return result;
}

Matrix4 Matrix4::transpose()
{
    Matrix4 tM = Matrix4();
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            tM[col][row] = core[row][col];
        }
    }
    return tM;
}

//Matrix4* Matrix4::inverse()
//{
//    //finding determinant
//    float determinant = 0;
//    for (int i = 0; i < 4; i++)
//        determinant = determinant + (core[0][i] * (core[1][(i + 1) % 4] * core[2][(i + 2) % 4] - core[1][(i + 2) % 4] * core[2][(i + 1) % 4]));
//
//    if (determinant == 0) {
//        return NULL;
//    }
//
//    Matrix4 inv = Matrix4();
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++)
//            inv[i][j] = ((core[(j + 1) % 3][(i + 1) % 3] * core[(j + 2) % 3][(i + 2) % 3]) - (core[(j + 1) % 3][(i + 2) % 3] * core[(j + 2) % 3][(i + 1) % 3])) / determinant;
//    }
//    return &inv;
//}
