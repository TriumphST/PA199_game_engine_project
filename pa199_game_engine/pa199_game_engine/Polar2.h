#pragma once
#include "Vector2.h"

class Polar2
{
public:
	float distance = 0.0f;
	float angle = 0.0f;

	Polar2();
	Polar2(float distance, float angle);
	Vector2 toCartesian();
};

