#include "Gameobject.h"
#include "Matrix4.h"
# define M_PI           3.14159265358979323846
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


Gameobject::Gameobject(Shader shaderProgram, Mesh *mesh, std::string textureName)
{
    this->mesh = mesh;
    this->triangles = triangles;
    this->textureName = textureName;
    position = Vector3(0.0f, 0.0f, 0.0f);
    rotation = Vector3(0.0f, 0.0f, 0.0f);
    scale = Vector3(1.0f, 1.0f, 1.0f);
    velocity = Vector3(0.0f, 0.0f, 0.0f);
    color = Vector3(1.0f, 0.0f, 0.0f);
    transparency = 1.0f;

    //this->vertices = vertices;

    //Shader shaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    this->shaderProgram = shaderProgram;

    // vertexes
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->GetMeshVertexes().size() * sizeof(float), &mesh->GetMeshVertexes()[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normals
    if (mesh->GetMeshNormals().size() > 0) {
        glGenBuffers(1, &normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetMeshNormals().size() * sizeof(float), &mesh->GetMeshNormals()[0], GL_STATIC_DRAW);
    }

    // indexes
    if(mesh->GetMeshIndexes().size() > 0){
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetMeshIndexes().size() * sizeof(unsigned int), &mesh->GetMeshIndexes()[0], GL_STATIC_DRAW);
    }

    if (mesh->GetMeshNormals().size() > 0) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(
            1,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
    }

    // tex coords
    if (textureName.empty() == false) {
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetMeshTexCoords().size() * sizeof(float), &mesh->GetMeshTexCoords()[0], GL_STATIC_DRAW);

        // load and create a texture 
        // -------------------------
        glGenTextures(1, &texturebuffer);
        glBindTexture(GL_TEXTURE_2D, texturebuffer); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        fs::path currentp = std::filesystem::current_path();
        std::string filepathstr = currentp.string() + "\\textures\\" + textureName;
        const char* filepath = filepathstr.c_str();
        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
}

float Gameobject::toRadians(float x) {
    return 2 * M_PI * (x / 360);
}

void Gameobject::update(float deltaTime)
{
    if (isActive == false){
        return;
    }
    // call ODE solver here
    this->position = position + velocity * deltaTime;
}

void Gameobject::render(float with, float height, int cameraMode)
{
    if (isActive == false) {
        return;
    }
    shaderProgram.use();

    // camera settings
    Vector3 cameraPos;
    if(cameraMode == 1){ // normal front
        cameraPos = Vector3(0.0f, 7.0f, 30.0f);
    }
    else if (cameraMode == 2) // top
    {
        cameraPos = Vector3(0.0f, 30.0f, 0.000001f);
    }
    else if (cameraMode == 3) // from right
    {
        cameraPos = Vector3(30.0f, 7.0f, 5.0f);
    }
    else if (cameraMode == 4)  // from left
    {
        cameraPos = Vector3(-30.0f, 7.0f, 5.0f);
    }
    else if (cameraMode == 5) // from bottom
    {
        cameraPos = Vector3(0.0f, -30.0f, 0.000001f);
    }
    Vector3 cameraTarget = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 cameraDirection = (cameraPos - cameraTarget).normalized();
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 cameraRight = up.cross(cameraDirection).normalized(); // glm::normalize(glm::cross(up, cameraDirection));
    Vector3 cameraUp = cameraDirection.cross(cameraRight); // glm::cross(cameraDirection, cameraRight);
    Matrix4 view = Matrix4::lookatMatrix(cameraRight, cameraUp, cameraDirection, cameraPos);

    // create transformations
    Matrix4 projection = Matrix4::perspectiveMatrix(toRadians(45.0f), with / height, 0.1f, 100.0f);

    // pass transformation matrices to the shader
    shaderProgram.setMat4("projection", projection.core);
    shaderProgram.setMat4("view", view.core);

    // calculate the model matrix for each object and pass it to shader before drawing
    Matrix4 transM = Matrix4::translationMatrix(position.x, position.y, position.z);
    Matrix4 scaleM = Matrix4::scaleMatrix(scale.x, scale.y, scale.z);
    Matrix4 rotM = Matrix4::rotationMatrix(rotation.x, rotation.y, rotation.z);

    //Matrix4 rotCenter = Matrix4::rotationMatrix(rotationAroundCenter.x, rotationAroundCenter.y, rotationAroundCenter.z);

    Matrix4 model = Matrix4(1.0f);
    model = model * transM * scaleM * rotM;
    shaderProgram.setMat4("model", model.core);
    float camPos[3] = { cameraPos.x, cameraPos.y, cameraPos.z };
    shaderProgram.setVec3("viewPos", camPos);

    int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "color");
    glUniform3f(vertexColorLocation, color.x, color.y, color.z);

    int vertexTransparencyLocation = glGetUniformLocation(shaderProgram.ID, "transparency");
    glUniform1f(vertexTransparencyLocation, transparency);

    glBindVertexArray(VAO);

    if (textureName.empty() == false) {
        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texturebuffer);
    }

    //glDrawArrays(GL_POINTS, 0, mesh.GetMeshVertexes().size());
    // Index buffer
    if (mesh->GetMeshIndexes().size() > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glDrawElements(
            GL_TRIANGLES,      // mode
            mesh->GetMeshIndexes().size(),    // count
            GL_UNSIGNED_INT,   // type
            (void*)0           // element array buffer offset
        );
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetMeshVertexes().size());
    }
}

void Gameobject::clean()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(shaderProgram.ID);
}

