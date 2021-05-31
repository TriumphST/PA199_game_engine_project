#include "Helper.h"

# define M_PI           3.14159265358979323846

float Helper::toRadians(float x) {
    return 2 * M_PI * (x / 360);
}

float Helper::toDegrees(float x) {
    return (x * 180)/ M_PI;
}

float Helper::getAngleY(Vector3 v1, Vector3 v2)
{
    //x,z is the plane
    float dot = v1.x * v2.x + v1.z * v2.z;      // dot product between[x1, y1] and [x2, y2]
    float det = v1.x * v2.z - v1.z * v2.x;      // determinant
    return atan2(det, dot);  // atan2(y, x) or atan2(sin, cos);
}
