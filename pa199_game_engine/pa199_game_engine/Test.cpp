#include "Test.h"
#include "Vector3.h"
#include "Matrix3.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

void Test::runTests()
{
    testVector3();
    testMatrix3();
    std::cout << "All tests passed" << std::endl;
}

bool isClose(float x, float y)
{
    int didgets = 1000;
    float roundX = std::round(x * didgets);
    float roundY = std::round(y * didgets);
    roundX = roundX / didgets;
    roundY = roundY / didgets;
    float abs_val = abs(roundX - roundY);
    if (abs_val <= 0.0001) 
    {
        return true;
    }
    else 
    {
        return false;
    }
}

void Test::testVector3()
{
    // empty
    Vector3 v = Vector3();
    assert(v.x == 0.0);
    assert(v.y == 0.0);
    assert(v.z == 0.0);

    // -
    Vector3 v1 = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 v2 = Vector3(1.0f, 1.0f, 1.0f);
    v = v1 - v2;
    assert(v.x == 0.0);
    assert(v.y == 0.0);
    assert(v.z == 0.0);

    // +
    v1 = Vector3(1.0f, 1.0f, 1.0f);
    v2 = Vector3(1.0f, 1.0f, 1.0f);
    v = v1 + v2;
    assert(v.x == 2.0);
    assert(v.y == 2.0);
    assert(v.z == 2.0);

    // normalize
    v1 = Vector3(2.0f, -4.0f, 1.0f);
    v = v1.normalized();
    assert(isClose(v.x, 2/sqrt(21)));
    assert(isClose(v.y, -4/sqrt(21)));
    assert(isClose(v.z, 1/sqrt(21)));

    v1 = Vector3(1.0f, 5.0f, -1.0f);
    v = v1.normalized();
    assert(isClose(v.x, 1 / (3*sqrt(3))));
    assert(isClose(v.y, 5 / (3 * sqrt(3))));
    assert(isClose(v.z, -1 / (3 * sqrt(3))));

    v1 = Vector3(0.0f, 0.0f, 0.0f);
    v = v1.normalized();
    assert(isClose(v.x, 0));
    assert(isClose(v.y, 0));
    assert(isClose(v.z, 0));
}

void Test::testMatrix3()
{
    // transpose
    float arrM[3][3] = { {1,2,3},{4,5,6},{7,8,9} };
    Matrix3 M = Matrix3(arrM);
    Matrix3 Mt = M.transpose();
    float arrA0[3][3] = { {1,4,7},{2,5,8},{3,6,9} };
    Matrix3 A = Matrix3(arrA0);
    assert(A == Mt);

    // inverse
    float arrM1[3][3] = { {3,0,2},{2,0,-2},{0,1,1} };
    M = Matrix3(arrM1);
    Matrix3* Minv = M.inverse();
    float arrA1[3][3] = { {0.2,0.2,0},{-0.2,0.3,1},{0.2,-0.3,0} };
    A = Matrix3(arrA1);
    assert(A == *Minv);

    float arrM2[3][3] = { {1,2,3},{0,1,4},{5,6,0} };
    M = Matrix3(arrM2);
    Minv = M.inverse();
    float arrA2[3][3] = { {-24,18,5},{20,-15,-4},{-5,4,1} };
    A = Matrix3(arrA2);
    assert(A == *Minv);

    float arrM3[3][3] = { {3,0,2},{2,0,-2},{0,1,1} };
    M = Matrix3(arrM3);
    Minv = M.inverse();
    float arrA3[3][3] = { {0.2,0.2,0},{-0.2,0.3,1},{0.2,-0.3,0} };
    A = Matrix3(arrA3);
    assert(A == *Minv);

    // inverse of singular matrix
    float arrM4[3][3] = { {2,1,2},{1,0,1},{4,1,4} };
    M = Matrix3(arrM4);
    Matrix3* Minvnull = M.inverse();
    assert(NULL == Minvnull);

    // multiply
    float arrA[3][3] = { {10,20,10},{4,5,6},{2,3,5} };
    A = Matrix3(arrA);
    float arrB[3][3] = { {3,2,4},{3,3,9},{4,4,2} };
    Matrix3 B = Matrix3(arrB);
    float arrABexp[3][3] = { {130,120,240},{51,47,73},{35,33,45} };
    Matrix3 ABexp = Matrix3(arrABexp);
    Matrix3 AB = A*B;
    assert(AB == ABexp);

    // add
    float arrA11[3][3] = { {10,20,10},{4,5,6},{2,3,5} };
    A = Matrix3(arrA11);
    float arrB11[3][3] = { {3,2,4},{3,3,9},{4,4,2} };
    B = Matrix3(arrB11);
    float arrABexp1[3][3] = { {13,22,14},{7,8,15},{6,7,7} };
    ABexp = Matrix3(arrABexp1);
    AB = A + B;
    assert(AB == ABexp);

    // substuct
    float arrA12[3][3] = { {10,20,10},{4,5,6},{2,3,5} };
    A = Matrix3(arrA12);
    float arrB12[3][3] = { {3,2,4},{3,3,9},{4,4,2} };
    B = Matrix3(arrB12);
    float arrABexp2[3][3] = { {7,18,6},{1,2,-3},{-2,-1,3} };
    ABexp = Matrix3(arrABexp2);
    AB = A - B;
    assert(AB == ABexp);
}
