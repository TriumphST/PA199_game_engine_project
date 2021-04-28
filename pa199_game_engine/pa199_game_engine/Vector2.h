#pragma once
#include "Polar2.h"

class Vector2
{
public:
	float x = 0.0f;
	float y = 0.0f;

	Vector2();
	Vector2(float x, float y);
	Polar2 toPolar();
};

