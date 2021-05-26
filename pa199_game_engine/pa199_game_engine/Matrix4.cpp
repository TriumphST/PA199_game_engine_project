#include "Matrix4.h"
#include <stdio.h>
#include <cmath>

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

Matrix4 Matrix4::operator*(const Vector3& other)
{
    Matrix4 results = Matrix4();
    float vec4[4] = { other.x, other.y, other.z, 1 };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            for (int u = 0; u < 4; u++)
                results[i][j] += core[i][u] * vec4[j];
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

Matrix4 Matrix4::scaleMatrix(float Sx, float Sy, float Sz)
{
    float scaleArr[4][4] = { 
        {Sx,0, 0, 0},
        {0, Sy,0, 0},
        {0, 0, Sz,0},
        {0, 0, 0, 1} 
    };
    Matrix4 scale = Matrix4(scaleArr);
    return scale;
}

Matrix4 Matrix4::translationMatrix(float Tx, float Ty, float Tz)
{
    float transArr[4][4] = {
        {1,0,0,Tx},
        {0,1,0,Ty},
        {0,0,1,Tz},
        {0,0,0, 1}
    };
    Matrix4 translation = Matrix4(transArr);
    return translation;
}

Matrix4 Matrix4::rotationMatrix(float Rx, float Ry, float Rz)
{
    float rotXArr[4][4] = {
        {1,       0,        0,0},
        {0, cos(Rx), -sin(Rx),0},
        {0, sin(Rx),  cos(Rx),0},
        {0,       0,        0,1}
    };
    float rotYArr[4][4] = {
        { cos(Ry), 0, sin(Ry), 0},
        {       0, 1,       0, 0},
        {-sin(Ry), 0, cos(Ry), 0},
        {       0, 0,       0, 1}
    };
    float rotZArr[4][4] = {
        {cos(Rz), -sin(Rz), 0, 0},
        {sin(Rz),  cos(Rz), 0, 0},
        {      0,        0, 1, 0},
        {      0,        0, 0, 1} 
    };
    Matrix4 rotX = Matrix4(rotXArr);
    Matrix4 rotY = Matrix4(rotYArr);
    Matrix4 rotZ = Matrix4(rotZArr);

    Matrix4 rotation = rotX * rotY * rotZ;
    return rotation;
}

Matrix4 Matrix4::lookatMatrix(Vector3 R, Vector3 U, Vector3 D, Vector3 P)
{
    float a[4][4] = {
        {R.x,R.y,R.z,0},
        {U.x,U.y,U.z,0},
        {D.x,D.y,D.z,0},
        {  0,  0,  0,1}
    };
    Matrix4 A = Matrix4(a);

    float b[4][4] = {
        {1,0,0,-P.x},
        {0,1,0,-P.y},
        {0,0,1,-P.z},
        {0,0,0, 1}
    };
    Matrix4 B = Matrix4(b);
    return A*B;
}

Matrix4 Matrix4::perspectiveMatrix(float angle, float ratio, float near, float far)
{
    float tan_half_angle = tan(angle / 2);
    float M11 = 1 / (ratio * tan_half_angle);
    float M22 = 1 / (tan_half_angle);
    float M33 = -(far + near) / (far - near);
    float M34 = -(2 * far * near) / (far - near);
    float persArr[4][4] = {
        {M11,   0,   0,   0},
        {  0, M22,   0,   0},
        {  0,   0, M33, M34},
        {  0,   0,   -1,  0}
    };
    Matrix4 perspective = Matrix4(persArr);
    return perspective;
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
