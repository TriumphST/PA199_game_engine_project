#pragma once
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
	//Matrix4 operator*(const Vector3& vec);
	Matrix4 operator+(const Matrix4& other);
	Matrix4 operator-(const Matrix4& other);
	Matrix4 transpose();
	//Matrix4* inverse();
};

