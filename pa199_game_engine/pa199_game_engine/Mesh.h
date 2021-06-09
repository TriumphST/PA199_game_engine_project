#pragma once
#include <vector>
#include <tuple>
#include "Vector3.h"
#include "Vector2.h"

class Mesh
{
public:
    Mesh() {};
    Mesh(std::vector<Vector3> vertices, std::vector<Vector3> normals, int numOfTriangles);
    Mesh(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<unsigned int> indexes = {}, std::vector<Vector2> texCoords = {});
    std::vector<float> GetMeshVertexes();
    std::vector<float> GetMeshNormals();
    std::vector<unsigned int> GetMeshIndexes();
    std::vector<float> GetMeshTexCoords();
    int GetNumOfTrinagles();
private:
    std::vector<float> verticesArr;
    std::vector<float> normalsArr;
    std::vector<float> texCoordsArr;
    std::vector<unsigned int> indexesArr;
    int numOfTriangles;
};