#pragma once
#include <vector>
#include <tuple>
#include "Vector3.h"

class Mesh
{
public:
    Mesh() {};
    Mesh(std::vector<Vector3> vertices, std::vector<unsigned int> indexes);
    std::vector<float> GetMeshVertexes();
    std::vector<unsigned int> GetMeshIndexes();
private:
    std::vector<float> verticesArr;
    std::vector<unsigned int> indexesArr;
};