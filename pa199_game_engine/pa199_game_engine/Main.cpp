#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vector3.h"
#include "Test.h"
#include "Shader.h"
#include "Matrix4.h"
#include "Gameobject.h"

# define M_PI           3.14159265358979323846

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int CUBE_COUNT = 2;
// world space positions of our cubes
Vector3 cubePositions[CUBE_COUNT] = {
    Vector3(-1.0f,  0.0f,  0.0f),
    Vector3(1.0f,  0.0f, 0.0f),
};

std::vector<float> cubeVertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLFWwindow* openWindow() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        assert(false);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        assert(false);
    }

    return window;
}

unsigned int * SetUpVertexData()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    //unsigned int VBO, VAO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);


    //// uncomment this call to draw in wireframe polygons.
    ////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //unsigned int buffers[2] = { VBO, VAO };
    return nullptr;
}

float toRadians(float x) {
    return 2 * M_PI * (x / 360);
}

void renderCube(Shader shaderProgram, unsigned int VAO) 
{
    shaderProgram.use();
}

void renderGOs(std::vector<Gameobject> GOs)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < GOs.size(); i++)
    {
        GOs[i].render((float)SCR_WIDTH, (float)SCR_HEIGHT);
    }
}

void render(Shader shaderProgram, unsigned int VAO) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.use();

    // create transformations
    Matrix4 projection = Matrix4::perspectiveMatrix(toRadians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    Matrix4 view = Matrix4::translationMatrix(0.0f, 0.0f, -3.0f);

    // pass transformation matrices to the shader
    shaderProgram.setMat4("projection", projection.core);
    shaderProgram.setMat4("view", view.core);

    // render boxes
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < CUBE_COUNT; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        Matrix4 trans = Matrix4::translationMatrix(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
        Matrix4 scale = Matrix4::scaleMatrix(0.5f, 0.5f, 0.5f);
        float angle = toRadians(20.0f * i);
        //Matrix4 rot = Matrix4::rotationMatrix(angle, 0.3f* angle, 0.5f* angle);
        Matrix4 model = Matrix4(1.0f);
        model = model * trans * scale;// * rot;
        shaderProgram.setMat4("model", model.core);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

int main()
{
    //Test t = Test();
    //t.runTests();
    //return 0;

    GLFWwindow* window = openWindow();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //Shader ourShader("shaders/coordinate_system.vs", "shaders/coordinate_system.fs");

    //unsigned int * buffers = SetUpVertexData();
    //unsigned int VBO = buffers[0];
    //unsigned int VAO = buffers[1];

    std::vector<Gameobject> GOs;

    Gameobject cube1 = Gameobject(cubeVertices, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject cube2 = Gameobject(cubeVertices, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    
    cube1.position = Vector3(-1.0f, 0.0f, 0.0f);
    cube2.position = Vector3(1.0f, 0.0f, 0.0f);

    GOs.push_back(cube1);
    //GOs.push_back(cube2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // rendering commands here
        //render(ourShader, VAO);
        //renderWithGLM(ourShader, VAO);
        renderGOs(GOs);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);

    for (int i = 0; i < GOs.size(); i++)
    {
        GOs[i].clean();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}