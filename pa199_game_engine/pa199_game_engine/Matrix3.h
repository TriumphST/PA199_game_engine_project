#pragma once
#include "Vector3.h"
class Matrix3
{
public:
	float core[3][3] = { 0 };
	bool isSingular = false;
	Matrix3();
	Matrix3(float init[3][3]);
	bool GetIsSingular();
	bool operator==(const Matrix3& other);
	float *operator[](const int& row);
	Matrix3 operator*(const Matrix3& scalar);
	Matrix3 operator+(const Matrix3& other);
	Matrix3 operator-(const Matrix3& other);
	Matrix3 transpose();
	Matrix3* inverse();
};

