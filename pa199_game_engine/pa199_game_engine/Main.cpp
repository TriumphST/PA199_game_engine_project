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
#include "Cylindrical3.h"
#include "Helper.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCradius_wallIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// game settings
struct settings {
    float radius_wall; // radius walls
    float diameter_wall; // diameter of walls
    float radius_paddles; // radius paddles
    float diameter_paddles; // diameter of paddles
    float phi_paddles; // angle (length) of paddles
    float paddleRotationSpeed; // angle (length) of paddles
    float radius_border; // radius border
    float ballSpeed;
} gameSettings;

int cameraMode = 1;
float paddleRotation = 0.0f;
bool isBallReadyToFire = true;

Gameobject sphere;


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
    GLFWwindow* window = glfwCreateWindow(SCradius_wallIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

void renderGOs(std::vector<Gameobject*> GOs)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < GOs.size(); i++)
    {
        GOs[i]->render((float)SCradius_wallIDTH, (float)SCR_HEIGHT, cameraMode);
    }
}

int angularDistance(int alpha, int beta) {
    int phi = std::abs(beta - alpha) % 360;       // This is either the distance or 360 - distance
    int distance = phi > 180 ? 360 - phi : phi;
    return distance;
}

void resetBall(Gameobject* sphere, Gameobject* paddleGO)
{
    Vector3 vectorCenter = paddleGO->position.normalized()*-1;
    sphere->position = paddleGO->position + vectorCenter;
    sphere->velocity = Vector3(0.0f);
    isBallReadyToFire = true;
}

void fireBall()
{
    if(isBallReadyToFire == false) {
        return;
    }
    Vector3 vectorCenter = sphere.position.normalized() * -gameSettings.ballSpeed;
    sphere.velocity = vectorCenter;
    isBallReadyToFire = false;
}

void checkCollisions(Gameobject* sphere, std::vector<Gameobject*> wallGOs, std::vector<Gameobject*> paddleGOs)
{
    // check broad collisions
    Cylindrical3 speherePos = sphere->position.toCylindrical();

    if (speherePos.distance > gameSettings.radius_border + (sphere->scale.x / 5))
    {
        // ball is out of game area
        resetBall(sphere, paddleGOs[0]);
    }

    // from center to wall radius + helf of wall diameter + half of shere radius
    if (speherePos.distance < gameSettings.radius_wall + (gameSettings.diameter_wall / 2) + (sphere->scale.x / 5))
    {
        // check wall collision
    }
    else if (speherePos.distance > gameSettings.radius_paddles - (gameSettings.diameter_paddles / 2) - (sphere->scale.x / 5))
    {
        // check paddle collision
        int closestIndex = 0;
        float closestAngle = 1000.0f;
        for (int i = 0; i < paddleGOs.size(); i++)
        {
            Gameobject* paddle = paddleGOs[i];
            Cylindrical3 paddlePos = paddle->position.toCylindrical();
            float angleDiff = angularDistance(Helper::toDegrees(speherePos.angle), Helper::toDegrees(paddlePos.angle));
            if (angleDiff < closestAngle) {
                closestAngle = angleDiff;
                closestIndex = i;
            }
        }

        // check closest paddle collision
        Gameobject* paddle = paddleGOs[closestIndex];
        Cylindrical3 paddlePos = paddle->position.toCylindrical();
        if (closestAngle <= gameSettings.phi_paddles) {
            // ball is colliding with the paddle
            float dir = 1.0f;
            if (speherePos.distance < gameSettings.radius_paddles) {
                dir = -1.0f;
            }
            Vector3 n = sphere->position.normalized() * dir;
            sphere->velocity = n*gameSettings.ballSpeed;
        }
    }
}

int main()
{
    //Test t = Test();
    //t.runTests();
    //return 0;

    // game settings
    gameSettings.radius_wall = 5.0f;
    gameSettings.diameter_wall = 1.0f;
    gameSettings.radius_paddles = 10.0f;
    gameSettings.diameter_paddles = 1.0f;
    gameSettings.phi_paddles = 20.0f;
    gameSettings.radius_border = 12.0f;
    gameSettings.ballSpeed = 0.03f;
    gameSettings.paddleRotationSpeed = 0.5f;

    GLFWwindow* window = openWindow();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //Shader ourShader("shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Shader ourShader("shaders/phong.vs", "shaders/phong.fs");

    std::vector<Gameobject*> GOs;

    Mesh triangleMesh = Mesh(triangleVertices, triangleIndexes);
    Mesh squereMesh = Mesh(squereVertices, squereIndexes);
    Mesh cubeMesh = Mesh(cubeVertices, cubeIndexes);
    Mesh sphereMesh = MeshGenerator::Sphere(1.0f);
    Mesh paddleMesh = MeshGenerator::Paddle(gameSettings.phi_paddles, 10);

    // triangle = Gameobject(ourShader, triangleMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject squere = Gameobject(ourShader, squereMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    //Gameobject cube = Gameobject(ourShader, cubeMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    //Gameobject cube1 = Gameobject(ourShader, cubeMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    sphere = Gameobject(ourShader, sphereMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject paddle1 = Gameobject(ourShader, paddleMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Gameobject paddle2 = Gameobject(ourShader, paddleMesh, "shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    
    squere.position = Vector3(-1.0f, 0.0f, 0.0f);
    squere.scale = Vector3(4.0f, 4.0f, 4.0f);
    squere.rotation = Vector3(Helper::toRadians(90.0f), 0.0f, 0.0f);
    sphere.position = Vector3(1.0f, 0.0f, 0.0f);
    //sphere.rotation = Vector3(Helper::toRadians(-90.0f), 0.0f, 0.0f);
    paddle1.position = Vector3(10.0f, 0.0f, 0.0f);
    //paddle1.rotation = Vector3(Helper::toRadians(-90.0f), 0.0f, 0.0f);
    paddle2.position = Vector3(-10.0f, 0.0f, 0.0f);
    paddle2.rotation = Vector3(0.0f, Helper::toRadians(180.0f), 0.0f);

    std::vector<Gameobject*> wallGOs;
    std::vector<Gameobject*> paddleGOs;

    paddleGOs.push_back(&paddle1);
    paddleGOs.push_back(&paddle2);

    GOs.push_back(&sphere);
    GOs.push_back(&paddle1);
    GOs.push_back(&paddle2);

    resetBall(&sphere, paddleGOs[0]);

    std::chrono::high_resolution_clock::time_point lastTick = std::chrono::high_resolution_clock::now();
    double dt;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        dt = (std::chrono::high_resolution_clock::now() - lastTick).count()/1000.0f;
        // input
        // -----
        processInput(window);

        checkCollisions(&sphere, wallGOs, paddleGOs);

        sphere.rotation = sphere.rotation + Vector3(0.0f, 0.0f, Helper::toRadians(1.0f * dt));

        for (int i = 0; i < paddleGOs.size(); i++)
        {
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, paddleRotation * Helper::toRadians(1.0f * dt), 0.0f);
            paddleGOs[i]->position = rotCenter * paddleGOs[i]->position;
        }

        for (int i = 0; i < GOs.size(); i++)
        {
            GOs[i]->update(dt);
        }

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
        GOs[i]->clean();
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
        
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        fireBall();
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        cameraMode = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        cameraMode = 2;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (paddleRotation == 0.0f)
            paddleRotation = -1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE){
        if (paddleRotation == -1.0f) 
            paddleRotation = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if(paddleRotation == 0.0f)
            paddleRotation = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        if (paddleRotation == 1.0f)
            paddleRotation = 0.0f;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}