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
    float arrA[3][3] = { {1,4,7},{2,5,8},{3,6,9} };
    Matrix3 A = Matrix3(arrA);
    assert(A == Mt);
}
