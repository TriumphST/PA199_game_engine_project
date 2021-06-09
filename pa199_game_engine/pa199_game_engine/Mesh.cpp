#include "Mesh.h"

Mesh::Mesh(std::vector<Vector3> vertices, std::vector<Vector3> normals, std::vector<unsigned int> indexes, std::vector<Vector2> texCoords)
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

	if (texCoords.size() > 0) {
		texCoordsArr = std::vector<float>(texCoords.size() * 2);
		for (int i = 0, j = 0; i < texCoords.size(); i++, j += 2)
		{
			texCoordsArr[j] = texCoords[i].x;
			texCoordsArr[j + 1] = texCoords[i].y;
		}
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

std::vector<float> Mesh::GetMeshTexCoords()
{
	return texCoordsArr;
}