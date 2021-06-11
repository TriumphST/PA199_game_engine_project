#include "Helper.h"

# define M_PI           3.14159265358979323846

float Helper::toRadians(float x) {
    return 2 * M_PI * (x / 360);
}

float Helper::toDegrees(float x) {
    return (x * 180)/ M_PI;
}

float Helper::distanceOnNumberLine(float x, float y)
{
    return fabs(x-y);
}

float Helper::getAngleY(Vector3 v1, Vector3 v2)
{
    //x,z is the plane
    float dot = v1.x * v2.x + v1.z * v2.z;      // dot product between[x1, y1] and [x2, y2]
    float det = v1.x * v2.z - v1.z * v2.x;      // determinant
    return atan2(det, dot);  // atan2(y, x) or atan2(sin, cos);
}


bool Helper::linePlaneIntersection(Vector3& contact, Vector3 ray, Vector3 rayOrigin, Vector3 normal, Vector3 coord) {
    
    // get d value
    float d = normal.dot(coord);

    if (normal.dot(ray) == 0.0f) {
        return false; // No intersection, the line is parallel to the plane
    }

    // Compute the X value for the directed line ray intersecting the plane
    float a = normal.dot(rayOrigin);
    float b = normal.dot(ray);
    float x = (d - a) / b;

    if (x < 0) {
        return false;
    }

    // output contact point
    contact = rayOrigin + ray.normalized() * x;

    return true;
}

bool Helper::isPointInsideAABB(Gameobject* aabb, Vector3 point)
{
    float distx = Helper::distanceOnNumberLine(point.x, aabb->position.x);
    float disty = Helper::distanceOnNumberLine(point.y, aabb->position.y);
    float distz = Helper::distanceOnNumberLine(point.z, aabb->position.z);
    if (distx <= aabb->position.x + (aabb->scale.x / 2.0f) &&
        disty <= aabb->position.y + (aabb->scale.y / 2.0f) &&
        distz <= aabb->position.z + (aabb->scale.z / 2.0f))
    {
        return true;
    }
    return false;
}
