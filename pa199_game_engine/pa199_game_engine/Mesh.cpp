#include "Mesh.h"

Mesh::Mesh(std::vector<Vector3> vertices, std::vector<unsigned int> indexes)
{
	this->indexesArr = indexes;
	verticesArr = std::vector<float>(vertices.size()*3);
	for (int i = 0, j = 0; i < vertices.size(); i++, j += 3)
	{
		verticesArr[j] = vertices[i].x;
		verticesArr[j+1] = vertices[i].y;
		verticesArr[j+2] = vertices[i].z;
	}
}

std::vector<float> Mesh::GetMeshVertexes()
{
	return verticesArr;
}

std::vector<unsigned int> Mesh::GetMeshIndexes()
{
	return indexesArr;
}