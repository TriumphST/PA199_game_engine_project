#pragma once
#include "Vector3.h"
#include <string>
#include <vector>
#include "Shader.h"

//enum MeshType { cube, sphere };

class Gameobject
{
public:
    Gameobject(std::vector<float> vertices, std::string vertexShaderPath, std::string fragmentShaderPath);

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Vector3 velocity;
    std::vector<float> vertices;
    
    void render(float with, float height);
    void clean();

private:
    Shader shaderProgram;

    GLuint VAO;
    GLuint VBO;
};

