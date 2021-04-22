#include "Matrix3.h"

Matrix3::Matrix3()
{
}

Matrix3::Matrix3(float init[3][3])
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            core[row][col] = init[row][col];
        }
    }
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

Matrix3 Matrix3::operator*(const Matrix3& scalar)
{
    return Matrix3();
}

Matrix3 Matrix3::operator+(const Matrix3& other)
{
    return Matrix3();
}

Matrix3 Matrix3::operator-(const Matrix3& other)
{
    return Matrix3();
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

Matrix3 Matrix3::inverse()
{
    return Matrix3();
}
