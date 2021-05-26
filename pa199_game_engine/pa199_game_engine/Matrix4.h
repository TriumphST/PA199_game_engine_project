#pragma once
#include "Vector3.h"

class Matrix4
{
public:
	float core[4][4] = { 0 };
	bool isSingular = false;
	Matrix4();
	Matrix4(float init[4][4]);
	Matrix4(float initNumber);
	bool GetIsSingular();
	bool operator==(const Matrix4& other);
	float* operator[](const int& row);
	Matrix4 operator*(const Matrix4& scalar);
	Matrix4 operator*(const Vector3& vec);
	Matrix4 operator+(const Matrix4& other);
	Matrix4 operator-(const Matrix4& other);
	Matrix4 transpose();
	//Matrix4* inverse();

	// transform
	static Matrix4 scaleMatrix(float Sx, float Sy, float Sz);
	static Matrix4 translationMatrix(float Tx, float Ty, float Tz);
	static Matrix4 rotationMatrix(float Rx, float Ry, float Rz);

	static Matrix4 lookatMatrix(Vector3 R, Vector3 U, Vector3 D, Vector3 P);

	// projection
	static Matrix4 perspectiveMatrix(float angle, float ratio, float near, float far);
};

