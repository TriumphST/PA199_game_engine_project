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
    float radius_wall;
    float diameter_wall;
    float radius_paddles;
    float diameter_paddles;
    float phi_paddles; // angle (length) of paddles
    float paddleRotationSpeed; // angle (length) of paddles
    float radius_border;
    float ballSpeed;
    int maxLives;
    int numOfWallSegments;
    int numOfWallFloors;
} gameSettings;

int cameraMode = 1;
float paddleRotation = 0.0f;
bool isBallReadyToFire = true;
int currentLives = gameSettings.maxLives;
bool isGameOver = false;
float phi_wall = 0.0f;
int wallHitCount = 0;

Gameobject * sphere;
std::vector<Gameobject*> wallGOs;
std::vector<Gameobject*> paddleGOs;
std::vector<Gameobject*> GOs;

Mesh paddleMesh;
Mesh triangleMesh;
Mesh squereMesh;
Mesh cubeMesh;
Mesh sphereMesh;
Mesh wallMesh;


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

void loadMeshes() {
    paddleMesh = MeshGenerator::Paddle(gameSettings.phi_paddles, 10, gameSettings.radius_paddles);
    triangleMesh = Mesh(triangleVertices, triangleIndexes);
    squereMesh = Mesh(squereVertices, squereIndexes);
    cubeMesh = Mesh(cubeVertices, cubeIndexes);
    sphereMesh = MeshGenerator::Sphere(1.0f);
    wallMesh = MeshGenerator::Paddle(phi_wall, 10, gameSettings.radius_wall);
}

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

void resetBall()
{
    Vector3 vectorCenter = paddleGOs[0]->position.normalized()*-1.5;
    sphere->position = paddleGOs[0]->position + vectorCenter;
    sphere->velocity = Vector3(0.0f);
    isBallReadyToFire = true;
}

void fireBall()
{
    if(isBallReadyToFire == false) {
        return;
    }
    srand((unsigned int)time(NULL));
    float a = 3.0f;
    float b = 2.0f;
    int posX = (rand() % 2);
    if (posX == 0)
        posX = -1;

    int posZ = (rand() % 2);
    if (posZ == 0)
        posZ = -1;

    float randomX = ((float(rand()) / float((RAND_MAX)) * a) + b) * posX;
    float randomZ = ((float(rand()) / float((RAND_MAX)) * a) + b) * posZ;
    Vector3 randomTargetPosition = Vector3(randomX,0.0f, randomZ);
    Vector3 vectorTarget = (sphere->position - randomTargetPosition).normalized() * -gameSettings.ballSpeed;
    sphere->velocity = vectorTarget;
    isBallReadyToFire = false;
}

void restartGame() {
    isGameOver = false;
    resetBall();
    currentLives = gameSettings.maxLives;
}

void moveWallsOneLevel() {
    for (int i = 0; i < wallGOs.size(); i++)
    {
        Vector3 newPos = wallGOs[i]->position;
        newPos.y -= 1.0f;
        wallGOs[i]->position = newPos;
    }
}

void gameWon() {
    std::cout << "Game Won!" << std::endl;
    std::cout << "Press ENTER to restart the game" << std::endl;
    isGameOver = true;
}

void wallHit(Gameobject* wall) {
    int a = -1;
    int b = -1;
    for (int i = 0; i < wallGOs.size(); i++)
    {
        if (wall == wallGOs[i])
            a = i;
    }
    wallGOs.erase(wallGOs.begin()+a);
    for (int i = 0; i < GOs.size(); i++)
    {
        if (wall == GOs[i])
            b = i;
    }
    GOs.erase(GOs.begin() + b);

    wallHitCount++;
    if (wallHitCount == gameSettings.numOfWallSegments) {
        wallHitCount = 0;
        moveWallsOneLevel();
    }

    if (wallGOs.size() <= 0) {
        gameWon();
    }
}

void checkCollisions(Gameobject* sphere, std::vector<Gameobject*> wallGOs, std::vector<Gameobject*> paddleGOs)
{
    // check broad collisions
    Cylindrical3 speherePos = sphere->position.toCylindrical();

    if (speherePos.distance > gameSettings.radius_border + (sphere->scale.x / 5))
    {
        // ball is out of game area
        currentLives--;
        std::cout << "Current lives: " << currentLives << std::endl;
        if (currentLives <= 0) {
            std::cout << "Game Over!" << std::endl;
            std::cout << "Press ENTER to restart the game" << std::endl;
            isGameOver = true;
        }
        resetBall();
    }
    // from center to wall radius + helf of wall diameter + half of shere radius
    else if (speherePos.distance < gameSettings.radius_wall + (gameSettings.diameter_wall / 2) + (sphere->scale.x / 2)
        && speherePos.distance > gameSettings.radius_wall - (gameSettings.diameter_wall / 2) - (sphere->scale.x / 2))
    {
        if (wallGOs.size() <= 0)
            return;
        // check wall collision
        // check paddle collision
        int closestIndex = 0;
        float closestAngle = 1000.0f;
        for (int i = 0; i < wallGOs.size(); i++)
        {
            Gameobject* wall = wallGOs[i];
            if (wall->position.y != 0.0f) { // skip those, that are not in corect level
                continue;
            }
            Cylindrical3 wallPos = wall->position.toCylindrical();
            float angleDiff = angularDistance(Helper::toDegrees(speherePos.angle), Helper::toDegrees(wallPos.angle));
            if (angleDiff < closestAngle) {
                closestAngle = angleDiff;
                closestIndex = i;
            }
        }

        // check closest wall collision
        Gameobject* wall = wallGOs[closestIndex];
        Cylindrical3 wallPos = wall->position.toCylindrical();
        if (closestAngle <= phi_wall) {
            // ball is colliding with the wall
            float dir = 1.0f;
            if (speherePos.distance < gameSettings.radius_wall) {
                dir = -1.0f;
            }
            Vector3 n = sphere->position.normalized() * dir;
            if (n.dot(sphere->velocity) >= 0.0f)
                return;
            Vector3 Vn = n * (n.dot(sphere->velocity));
            sphere->velocity = sphere->velocity - (Vn * 2);
            wallHit(wall);
        }
    }
    else if (speherePos.distance > gameSettings.radius_paddles - (gameSettings.diameter_paddles / 2) - (sphere->scale.x / 2))
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
            if (n.dot(sphere->velocity) >= 0.0f)
                return;
            Vector3 Vn = n * (n.dot(sphere->velocity));
            sphere->velocity = sphere->velocity - Vn*2;
        }
    }
}

void createPaddles(Shader shaderProgram) {
    Gameobject * paddle1 = new Gameobject(shaderProgram, &paddleMesh);
    Gameobject * paddle2 = new Gameobject(shaderProgram, &paddleMesh);
    Gameobject * paddle3 = new Gameobject(shaderProgram, &paddleMesh);
    paddle1->position = Vector3(10.0f, 0.0f, 0.0f);
    paddle2->position = Vector3(10.0f, 0.0f, 0.0f);
    paddle3->position = Vector3(10.0f, 0.0f, 0.0f);

    Matrix4 rotCenter1 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(0.0f), 0.0f);
    paddle1->position = rotCenter1 * paddle1->position;

    Matrix4 rotCenter2 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(120.0f), 0.0f);
    paddle2->position = rotCenter2 * paddle2->position;

    Matrix4 rotCenter3 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(240.0f), 0.0f);
    paddle3->position = rotCenter3 * paddle3->position;

    paddleGOs.push_back(paddle1);
    paddleGOs.push_back(paddle2);
    paddleGOs.push_back(paddle3);

    GOs.push_back(paddle1);
    GOs.push_back(paddle2);
    GOs.push_back(paddle3);
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
    gameSettings.ballSpeed = 0.1f;
    gameSettings.paddleRotationSpeed = 0.5f;
    gameSettings.maxLives = 3;
    gameSettings.numOfWallSegments = 10;
    gameSettings.numOfWallFloors = 4;
    gameSettings.radius_wall = 5.0f;

    currentLives = gameSettings.maxLives;
    phi_wall = (360.0f / float(gameSettings.numOfWallSegments)) / 2.0f;

    loadMeshes();

    std::cout << "Game starts!" << std::endl;
    std::cout << "Current lives: " << currentLives << std::endl;

    GLFWwindow* window = openWindow();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //Shader ourShader("shaders/coordinate_system.vs", "shaders/coordinate_system.fs");
    Shader ourShader("shaders/phong.vs", "shaders/phong.fs");

    //Gameobject squere = Gameobject(ourShader, squereMesh);
    sphere = new Gameobject(ourShader, &sphereMesh);

    
    sphere->position = Vector3(1.0f, 0.0f, 0.0f);
    sphere->color = Vector3(170.0f / 255.0f, 174.0f / 255.0f, 181.0f / 255.0f);

    GOs.push_back(sphere);

    //createPaddles(ourShader);
    //Gameobject paddle1 = Gameobject(ourShader, &paddleMesh);
    //Gameobject paddle2 = Gameobject(ourShader, &paddleMesh);
    //Gameobject paddle3 = Gameobject(ourShader, &paddleMesh);
    //paddle1.position = Vector3(10.0f, 0.0f, 0.0f);
    //paddle2.position = Vector3(10.0f, 0.0f, 0.0f);
    //paddle3.position = Vector3(10.0f, 0.0f, 0.0f);

    //Matrix4 rotCenter1 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(0.0f), 0.0f);
    //paddle1.position = rotCenter1 * paddle1.position;

    //Matrix4 rotCenter2 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(120.0f), 0.0f);
    //paddle2.position = rotCenter2 * paddle2.position;

    //Matrix4 rotCenter3 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(240.0f), 0.0f);
    //paddle3.position = rotCenter3 * paddle3.position;

    //paddleGOs.push_back(&paddle1);
    //paddleGOs.push_back(&paddle2);
    //paddleGOs.push_back(&paddle3);

    //GOs.push_back(&paddle1);
    //GOs.push_back(&paddle2);
    //GOs.push_back(&paddle3);

    Gameobject* paddle1 = new Gameobject(ourShader, &paddleMesh);
    Gameobject* paddle2 = new Gameobject(ourShader, &paddleMesh);
    Gameobject* paddle3 = new Gameobject(ourShader, &paddleMesh);
    paddle1->position = Vector3(10.0f, 0.0f, 0.0f);
    paddle2->position = Vector3(10.0f, 0.0f, 0.0f);
    paddle3->position = Vector3(10.0f, 0.0f, 0.0f);
    paddle1->color = Vector3(66.0f/255.0f, 135.0f / 255.0f, 245.0f / 255.0f);
    paddle2->color = Vector3(66.0f /255.0f, 135.0f / 255.0f, 245.0f / 255.0f);
    paddle3->color = Vector3(66.0f /255.0f, 135.0f / 255.0f, 245.0f / 255.0f);

    Matrix4 rotCenter1 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(0.0f), 0.0f);
    paddle1->position = rotCenter1 * paddle1->position;

    Matrix4 rotCenter2 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(120.0f), 0.0f);
    paddle2->position = rotCenter2 * paddle2->position;

    Matrix4 rotCenter3 = Matrix4::rotationMatrix(0.0f, Helper::toRadians(240.0f), 0.0f);
    paddle3->position = rotCenter3 * paddle3->position;

    paddleGOs.push_back(paddle1);
    paddleGOs.push_back(paddle2);
    paddleGOs.push_back(paddle3);

    GOs.push_back(paddle1);
    GOs.push_back(paddle2);
    GOs.push_back(paddle3);

    //createWalls(ourShader);
    for (int f = 0; f < gameSettings.numOfWallFloors; f++)
    {
        for (int s = 0; s < gameSettings.numOfWallSegments; s++)
        {
            // create
            Gameobject * wall = new Gameobject(ourShader, &wallMesh);
            // find poition on the circle
            wall->position = Vector3(gameSettings.radius_wall, f, 0.0f);
            float angle = (phi_wall * 2.0f) * float(s);
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, Helper::toRadians(angle), 0.0f);
            wall->position = rotCenter * wall->position;
            if((f+s)%2 == 1){
                wall->color = Vector3(65.0f / 255.0f, 171.0f / 255.0f, 51.0f / 255.0f);
            }
            else {
                wall->color = Vector3(207.0f / 255.0f, 149.0f / 255.0f, 14.0f / 255.0f);
            }
            float rad = Helper::toRadians(angle);
            Cylindrical3 test = wall->position.toCylindrical();
            // rotate towards center
            float angleY = Helper::getAngleY(Vector3(1.0f, 0.0f, 0.0f), wall->position.normalized());
            wall->rotation.y = -angleY;
            GOs.push_back(wall);
            wallGOs.push_back(wall);
        }
    }

    resetBall();

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

        checkCollisions(sphere, wallGOs, paddleGOs);

        //sphere->rotation = sphere->rotation + Vector3(0.0f, 0.0f, Helper::toRadians(1.0f * dt));

        if (isBallReadyToFire == true) 
        {
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, paddleRotation * Helper::toRadians(1.0f * dt), 0.0f);
            sphere->position = rotCenter * sphere->position;
        }

        for (int i = 0; i < paddleGOs.size(); i++)
        {
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, paddleRotation * Helper::toRadians(1.0f * dt), 0.0f);
            paddleGOs[i]->position = rotCenter * paddleGOs[i]->position;
            float angleY = Helper::getAngleY(Vector3(1.0f, 0.0f, 0.0f), paddleGOs[i]->position.normalized());
            //std::cout << i << ": " << Helper::toDegrees(angleY) << std::endl;
            paddleGOs[i]->rotation.y = -angleY;
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

void closeApplication()
{
    for (int i = 0; i < GOs.size(); i++)
    {
        delete GOs[i];
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        //closeApplication();
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (isGameOver == true) {
            restartGame();
        }
    }

    if (isGameOver == true) {
        return;
    }
        
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