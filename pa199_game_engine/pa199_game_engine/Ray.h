#pragma once
#include "Vector3.h"

class Ray
{
public:
    Vector3 P;
    Vector3 v;

    Ray(Vector3 P, Vector3 v);

    float distance(Ray r2);
    float distance(Vector3 Q);
    Vector3 closest(Vector3 Q);
};

