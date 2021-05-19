#pragma once
#include "Vector3.h"
#include <string>
#include <vector>
#include "Shader.h"

//enum MeshType { cube, sphere };

class Gameobject
{
public:
    Gameobject(Shader shaderProgram, std::vector<float> vertices, int stride, int triangles, std::string vertexShaderPath, std::string fragmentShaderPath);

    float toRadians(float x);

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Vector3 velocity;
    std::vector<float> vertices;
    
    void render(float with, float height);
    void clean();

private:
    int triangles = 0;
    Shader shaderProgram;

    GLuint VAO;
    GLuint VBO;
};

