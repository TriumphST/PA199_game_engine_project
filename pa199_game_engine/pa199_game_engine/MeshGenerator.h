#pragma once
#include <vector>
#include "Vector3.h"
#include "Cylindrical3.h"
#include "Mesh.h"
#include "Helper.h"

class MeshGenerator
{
public:
    static Mesh Triangle();
    static Mesh Circle(float radius, int resolution);
    static Mesh Squere();
    static Mesh Cube();
    static Mesh Sphere(float radius);
    static Mesh Paddle(float phi, int numOfLineVertexes, float distance);
};