#pragma once
#include <math.h>  
#include "Vector3.h"

class Helper
{
public:
    static float toRadians(float x);
    static float toDegrees(float x);
    static float getAngleY(Vector3 v1, Vector3 v2);
};

