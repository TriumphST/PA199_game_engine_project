#pragma once
#include "Cylindrical3.h"

class Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float all);

	float magnitude();
	Vector3 normalized();
	Vector3 opposite();
	bool operator==(const Vector3& other);
	Vector3 operator*(const float& scalar);
	Vector3 operator+(const Vector3& other);
	Vector3 operator-(const Vector3& other);
	Vector3 cross(const Vector3 other);
	float dot(const Vector3 other);
	Cylindrical3 toCylindrical();
};

