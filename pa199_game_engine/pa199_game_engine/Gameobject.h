#pragma once
#include "Vector3.h"
#include <string>
#include <vector>
#include "Shader.h"
#include "Mesh.h"
//#include "Texture.h"

//enum MeshType { cube, sphere };

class Gameobject
{
public:
    Gameobject() {};
    Gameobject(Shader shaderProgram, Mesh * mesh, std::string textureName = "");

    float toRadians(float x);

    Vector3 position;
    Vector3 rotation;
    Vector3 rotationAroundCenter;
    Vector3 scale;
    Vector3 velocity;
    Vector3 color;
    float transparency;
    std::vector<float> vertices;
    //Texture texture;
    std::string textureName;
    
    void update(float deltaTime);
    void render(float with, float height, int cameraMode);
    void clean();

private:
    int triangles = 0;
    Shader shaderProgram;
    Mesh * mesh;

    GLuint VAO;
    GLuint VBO;
    GLuint elementbuffer;
    GLuint normalbuffer;
    GLuint texturebuffer;
};

