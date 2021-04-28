#include "Polar2.h"
#include "Vector2.h"
#include <math.h>

Polar2::Polar2()
{
}

Polar2::Polar2(float distance, float angle)
{
    this->distance = distance;
    this->angle = angle;
}

Vector2 Polar2::toCartesian()
{
    float x = distance * cos(angle);
    float y = distance * sin(angle);
    Vector2 res = Vector2(x, y);
    return res;
}
