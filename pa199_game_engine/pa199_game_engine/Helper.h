#pragma once
#include <math.h>  
#include "Vector3.h"
#include "Gameobject.h"

class Helper
{
public:
    static float toRadians(float x);
    static float toDegrees(float x);
    static float distanceOnNumberLine(float x, float y);
    static float getAngleY(Vector3 v1, Vector3 v2);
    static bool linePlaneIntersection(Vector3& contact, Vector3 ray, Vector3 rayOrigin, Vector3 normal, Vector3 coord);
    static bool isPointInsideAABB(Gameobject * AABB, Vector3 point);
};

