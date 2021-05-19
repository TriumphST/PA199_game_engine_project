#include "Gameobject.h"
#include "Matrix4.h"
# define M_PI           3.14159265358979323846

Gameobject::Gameobject(Shader shaderProgram, std::vector<float> vertices, int stride, int triangles, std::string vertexShaderPath, std::string fragmentShaderPath)
{
    this->triangles = triangles;
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);

    this->vertices = vertices;

    //Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    this->shaderProgram = shaderProgram;

    //sending data to opengl
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

float Gameobject::toRadians(float x) {
    return 2 * M_PI * (x / 360);
}

void Gameobject::render(float with, float height)
{
    shaderProgram.use();

    // create transformations
    Matrix4 projection = Matrix4::perspectiveMatrix(toRadians(45.0f), with / height, 0.1f, 100.0f);
    Matrix4 view = Matrix4::translationMatrix(0.0f, 0.0f, -3.0f);

    // pass transformation matrices to the shader
    shaderProgram.setMat4("projection", projection.core);
    shaderProgram.setMat4("view", view.core);

    glBindVertexArray(VAO);
    // calculate the model matrix for each object and pass it to shader before drawing
    Matrix4 transM = Matrix4::translationMatrix(position.x, position.y, position.z);
    Matrix4 scaleM = Matrix4::scaleMatrix(scale.x, scale.y, scale.z);
    Matrix4 rotM = Matrix4::rotationMatrix(rotation.x, rotation.y, rotation.z);

    Matrix4 model = Matrix4(1.0f);
    model = model * transM * scaleM;// *rotM;
    shaderProgram.setMat4("model", model.core);

    glDrawArrays(GL_TRIANGLES, 0, triangles);
}

void Gameobject::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.ID);
}

