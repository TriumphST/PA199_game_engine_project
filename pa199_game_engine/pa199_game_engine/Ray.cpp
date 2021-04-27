#include "Ray.h"

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

    return d;
}
