#include "Ray.h"
#include <cmath>

Ray::Ray(Vector3 P, Vector3 v)
{
    this->P = P;
    this->v = v;
}

float Ray::distance(Ray p2)
{
    Ray p1 = *this;
    Vector3 n = p1.v.cross(p2.v);

    float d = (n.dot(p1.P - p2.P)) / n.magnitude();

    return std::abs(d);
}

float Ray::distance(Vector3 Q)
{
    float d = ((Q - P).cross(v).magnitude()) / v.magnitude();
    return std::abs(d);
}

Vector3 Ray::closest(Vector3 Q)
{
    Vector3 lineDir = v.normalized();
    Vector3 u = Q - P;
    float d = u.dot(lineDir);
    return P + lineDir * d;
}
