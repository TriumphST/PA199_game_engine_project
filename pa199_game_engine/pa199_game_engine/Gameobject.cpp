#include "Gameobject.h"
#include "Matrix4.h"
# define M_PI           3.14159265358979323846

Gameobject::Gameobject(Shader shaderProgram, Mesh mesh, std::string vertexShaderPath, std::string fragmentShaderPath)
{
    this->mesh = mesh;
    this->triangles = triangles;
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);

    //this->vertices = vertices;

    //Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    this->shaderProgram = shaderProgram;

    // vertexes
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.GetMeshVertexes().size() * sizeof(float), &mesh.GetMeshVertexes()[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // indexes
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetMeshIndexes().size() * sizeof(unsigned int), &mesh.GetMeshIndexes()[0], GL_STATIC_DRAW);
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

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glDrawElements(
        GL_TRIANGLES,      // mode
        mesh.GetMeshIndexes().size(),    // count
        GL_UNSIGNED_INT,   // type
        (void*)0           // element array buffer offset
    );
}

void Gameobject::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(shaderProgram.ID);
}

