#include "Cylindrical3.h"
#include <math.h>

Cylindrical3::Cylindrical3()
{
}

Cylindrical3::Cylindrical3(float distance, float angle, float z)
{
    this->distance = distance;
    this->angle = angle;
    this->z = z;
}

Vector3 Cylindrical3::toCartesian()
{
    float x = distance * cos(angle);
    float y = distance * sin(angle);
    Vector3 res = Vector3(x, y, z);
    return res;
}
