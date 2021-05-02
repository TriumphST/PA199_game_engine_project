#include "Gameobject.h"
#include "Matrix4.h"
# define M_PI           3.14159265358979323846

Gameobject::Gameobject(std::vector<float> vertices, std::string vertexShaderPath, std::string fragmentShaderPath)
{
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);

    this->vertices = vertices;

    Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    //sending data to opengl
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]), &this->vertices[0], GL_STATIC_DRAW);
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

    // calculate the model matrix for each object and pass it to shader before drawing
    Matrix4 transM = Matrix4::translationMatrix(position.x, position.y, position.z);
    Matrix4 scaleM = Matrix4::scaleMatrix(scale.x, scale.y, scale.z);
    Matrix4 rotM = Matrix4::rotationMatrix(rotation.x, rotation.y, rotation.z);

    Matrix4 model = Matrix4(1.0f);
    model = model * transM * scaleM * rotM;
    shaderProgram.setMat4("model", model.core);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Gameobject::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.ID);
}

