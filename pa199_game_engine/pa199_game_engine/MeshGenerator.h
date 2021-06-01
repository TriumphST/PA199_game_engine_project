#pragma once
#include <vector>
#include "Vector3.h"
#include "Cylindrical3.h"
#include "Mesh.h"
#include "Helper.h"

class MeshGenerator
{
public:
    static Mesh Sphere(float radius);
    static Mesh Paddle(float phi, int numOfLineVertexes, float distance);
    //static std::vector<float> SphereGLTriangles(float radius);
};

