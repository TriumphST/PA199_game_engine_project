#include "Vector2.h"
#include <math.h>

Vector2::Vector2()
{
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Polar2 Vector2::toPolar()
{
	float r = sqrt((x * x) + (y * y));
	float angle = atan(y/x);
	Polar2 res = Polar2(r, angle);
	return res;
}