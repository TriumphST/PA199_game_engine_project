#pragma once
#include "Vector3.h"

class Cylindrical3
{
public:
	float distance = 0.0f;
	float angle = 0.0f;
	float z = 0.0f;

	Cylindrical3();
	Cylindrical3(float distance, float angle, float z);
	Vector3 toCartesian();
};