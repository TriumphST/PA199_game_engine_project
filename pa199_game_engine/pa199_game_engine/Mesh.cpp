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

Mesh::Mesh(std::vector<Vector3> vertices, std::vector<unsigned int> indexes, std::vector<Vector3> normals)
{
	this->indexesArr = indexes;

	verticesArr = std::vector<float>(vertices.size() * 3);
	for (int i = 0, j = 0; i < vertices.size(); i++, j += 3)
	{
		verticesArr[j] = vertices[i].x;
		verticesArr[j + 1] = vertices[i].y;
		verticesArr[j + 2] = vertices[i].z;
	}

	normalsArr = std::vector<float>(normals.size() * 3);
	for (int i = 0, j = 0; i < normals.size(); i++, j += 3)
	{
		normalsArr[j] = normals[i].x;
		normalsArr[j + 1] = normals[i].y;
		normalsArr[j + 2] = normals[i].z;
	}
}

std::vector<float> Mesh::GetMeshVertexes()
{
	return verticesArr;
}

std::vector<float> Mesh::GetMeshNormals()
{
	return normalsArr;
}

std::vector<unsigned int> Mesh::GetMeshIndexes()
{
	return indexesArr;
}