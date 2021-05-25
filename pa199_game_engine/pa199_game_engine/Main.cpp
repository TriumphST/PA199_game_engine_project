#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vector3.h"
#include "Test.h"
#include "Shader.h"
#include "Matrix4.h"
#include "Gameobject.h"
#include "MeshGenerator.h"
#include "Mesh.h"

# define M_PI           3.14159265358979323846

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::vector<Vector3> squereVertices = {
        Vector3(-0.5f, -0.5f, 0.0f),   //0
        Vector3(0.5f, -0.5f, 0.0f),    //1
        Vector3(0.5f,  0.5f, 0.0f),   //2
        Vector3(-0.5f,  0.5f, 0.0f),    //3
};

std::vector<unsigned int> squereIndexes = {
        0,1,2,
        0,2,3
};

std::vector<Vector3> triangleVertices = {
    Vector3(-0.5f, -0.5f, 0.0f),
    Vector3(0.5f, -0.5f, 0.0f),
    Vector3(0.0f,  0.5f, 0.0f)
};

std::vector<unsigned int> triangleIndexes = {
    0, 1, 2
};

std::vector<Vector3> cubeVertices = {
        Vector3(-0.5f, -0.5f, 0.5f),
        Vector3(0.5f, -0.5f, 0.5f),
        Vector3(0.5f,  0.5f, 0.5f),
        Vector3(-0.5f,  0.5f, 0.5f),
        Vector3(-0.5f, -0.5f, -0.5f),
        Vector3(0.5f, -0.5f, -0.5f),
        Vector3(0.5f,  0.5f, -0.5f),
        Vector3(-0.5f,  0.5f, -0.5f),
};

std::vector<unsigned int> cubeIndexes = {
    0, 1, 2,
    0, 2, 3,

    0, 1, 5,
    0, 5, 4,

    0, 4, 7,
    0, 7, 3,

    3, 6, 7,
    3, 6, 2,

    4, 5, 6,
    4, 6, 7,

    1, 6, 2,
    1, 5, 6
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

float toRadians(float x) {
    return 2 * M_PI * (x / 360);
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

int main()
{
    //Test t = Test();
    //t.runTests();
    //return 0;

    GLFWwindow* window = openWindow();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/coordinate_system.vs", "shaders/coordinate_system.fs");

    std::vector<Gameobject> GOs;

    Mesh triangleMesh = Mesh(triangleVertices, triangleIndexes);
    Mesh squereMesh = Mesh(squereVertices, squereIndexes);
    Mesh cubeMesh = Mesh(cubeVertices, cubeIndexes);
    Mesh sphereMesh = MeshGenerator::Sphere(1.0f);

    Gameobject triangle = Gameobject(ourShader, triangleMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject squere = Gameobject(ourShader, squereMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject cube = Gameobject(ourShader, cubeMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    //Gameobject cube1 = Gameobject(ourShader, cubeVertices, 5, 36, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject sphere = Gameobject(ourShader, sphereMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    
    triangle.position = Vector3(0.0f, 1.0f, 0.0f);
    triangle.scale = Vector3(0.5f, 0.5f, 0.5f);
    squere.position = Vector3(-1.0f, 0.0f, 0.0f);
    squere.scale = Vector3(0.5f, 0.5f, 0.5f);
    squere.rotation = Vector3(0.0f, 0.0f, toRadians(30.0f));
    cube.position = Vector3(1.0f, 0.0f, 0.0f);
    cube.scale = Vector3(0.5f, 0.5f, 0.5f);
    cube.rotation = Vector3(0.0f, -toRadians(45.0f), -toRadians(20.0f));
    sphere.position = Vector3(0.0f, 0.0f, 0.0f);
    sphere.scale = Vector3(0.5f, 0.5f, 0.5f);
    sphere.rotation = Vector3(toRadians(-90.0f), 0.0f, 0.0f);



    std::chrono::high_resolution_clock::time_point lastTick = std::chrono::high_resolution_clock::now();
    double dt;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        dt = (std::chrono::high_resolution_clock::now() - lastTick).count()/1000;
        // input
        // -----
        processInput(window);

        sphere.rotation = sphere.rotation + Vector3(0.0f, 0.0f, toRadians(1.0f * dt));
        GOs.clear();
        GOs.push_back(triangle);
        GOs.push_back(squere);
        GOs.push_back(cube);
        GOs.push_back(sphere);

        // rendering commands here
        renderGOs(GOs);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        lastTick = std::chrono::high_resolution_clock::now();
    }

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