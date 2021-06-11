#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>

#include "Vector3.h"
#include "Test.h"
#include "Shader.h"
#include "Matrix4.h"
#include "Gameobject.h"
#include "MeshGenerator.h"
#include "Mesh.h"
#include "Cylindrical3.h"
#include "Helper.h"
#include "Ray.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
    int numOfBalls;
    float betweenLevelsTime;
} gameSettings;

int cameraMode = 1;
float paddleRotation = 0.0f;
bool isBallReadyToFire = true;
float currentBallSpeed;
int currentLives = gameSettings.maxLives;
bool isGameOver = false;
float phi_wall = 0.0f;
int wallHitCount = 0;
int score = 0;
int level = 1;
int currentBallsOut = 0;

bool newLevelTimer = false;
float timer = 0.0f;

std::vector <Gameobject*> sphereGOs;
std::vector <Gameobject*> aabbGOs;
std::vector<Gameobject*> wallGOs;
std::vector<Gameobject*> paddleGOs;
std::vector<Gameobject*> GOs;

Mesh paddleMesh;
Mesh triangleMesh;
Mesh squereMesh;
Mesh circleMesh;
Mesh cubeMesh;
Mesh sphereMesh;
Mesh wallMesh;

Texture Sand;


void loadMeshes() {
    paddleMesh = MeshGenerator::Paddle(gameSettings.phi_paddles, 10, gameSettings.radius_paddles);
    triangleMesh = MeshGenerator::Triangle();
    squereMesh = MeshGenerator::Squere();
    circleMesh = MeshGenerator::Circle(15.0f, 100);
    cubeMesh = MeshGenerator::Cube();
    sphereMesh = MeshGenerator::Sphere(0.5f);
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
    GLFWwindow* window = glfwCreateWindow(SCradius_wallIDTH, SCR_HEIGHT, "BouncyGame", NULL, NULL);
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

void resetBalls()
{
    for (int i = 0; i < sphereGOs.size(); i++)
    {
        Vector3 vectorCenter = paddleGOs[i]->position.normalized() * -1.5;
        sphereGOs[i]->position = paddleGOs[i]->position + vectorCenter;
        sphereGOs[i]->velocity = Vector3(0.0f);
        sphereGOs[i]->isActive = true;
        isBallReadyToFire = true;
    }

}

void fireBall(bool shootRandomly = true)
{
    if(isBallReadyToFire == false) {
        return;
    }
    for (int i = 0; i < sphereGOs.size(); i++)
    {
        if(shootRandomly){
        srand((unsigned int)time(NULL));
        float a = 2.5f;
        float b = 2.0f;
        int posX = (rand() % 2);
        if (posX == 0)
            posX = -1;

        int posZ = (rand() % 2);
        if (posZ == 0)
            posZ = -1;

        float randomX = ((float(rand()) / float((RAND_MAX)) * a) + b) * posX;
        float randomZ = ((float(rand()) / float((RAND_MAX)) * a) + b) * posZ;
        Vector3 randomTargetPosition = Vector3(randomX, 0.0f, randomZ);
        Vector3 vectorTarget = (sphereGOs[i]->position - randomTargetPosition).normalized() * -currentBallSpeed;
        sphereGOs[i]->velocity = vectorTarget;
        }
        else {
            sphereGOs[i]->velocity = sphereGOs[i]->position.normalized() * -currentBallSpeed;
        }
    }

    isBallReadyToFire = false;
}

void resetWall() {
    for (int f = 0; f < gameSettings.numOfWallFloors; f++)
    {
        for (int s = 0; s < gameSettings.numOfWallSegments; s++)
        {
            int index = (gameSettings.numOfWallSegments * f) + s;
            wallGOs[index]->position.y = float(f);
            wallGOs[index]->isActive = true;
            if (f == 0) {
                wallGOs[index]->transparency = 1.0f;
            }
            else {
                wallGOs[index]->transparency = 0.5f;
            }
        }
    }
}

void restartGame() {
    isGameOver = false;
    resetBalls();
    resetWall();
    currentLives = gameSettings.maxLives;
    currentBallSpeed = gameSettings.ballSpeed;
    level = 1;
    score = 0;
}

void moveWallsOneLevel(int row) {
    for (int i = row; i < wallGOs.size(); i+=gameSettings.numOfWallSegments)
    {
        Vector3 newPos = wallGOs[i]->position;
        newPos.y -= 1.0f;
        wallGOs[i]->position = newPos;   
    }
    if(row+ gameSettings.numOfWallSegments < wallGOs.size()){
        wallGOs[row + gameSettings.numOfWallSegments]->transparency = 1.0f;
    }
}

void gameWon() {
    std::cout << "Game Won!" << std::endl;
    std::cout << "Press ENTER to restart the game" << std::endl;
    isGameOver = true;
}

void printStatus() {
    cout << "Lives: " << currentLives << " " << "Level: " << level << " " << "Score: " << score << endl;
}

void startNewLevelTimer()
{
    timer = 0.0f;
    newLevelTimer = true;
}

void wallHit(Gameobject* wall, Gameobject* sphere) {
    if (wall->isActive == false) {
        return;
    }

    int a = -1;
    for (int i = 0; i < wallGOs.size(); i++)
    {
        if (wall == wallGOs[i])
            a = i;
    }
    wallGOs[a]->isActive = false;

    wallHitCount++;
    moveWallsOneLevel(a);
    score += 1 + level - 1;

    if (wallHitCount >= gameSettings.numOfWallFloors*gameSettings.numOfWallSegments) 
    {
        
        wallHitCount = 0;
        level++;
        currentBallSpeed += 2.0f;
        sphere->velocity = sphere->velocity.normalized() * currentBallSpeed;
        startNewLevelTimer();
    }
    printStatus();
}

void bounceSphere(Vector3 normal, Gameobject* sphere) {
    if (normal.dot(sphere->velocity) >= 0.0f)
        return;
    Vector3 Vn = normal * (normal.dot(sphere->velocity));
    sphere->velocity = sphere->velocity - Vn * 2;
}

void checkSphereAABBCollisions() {
    for (int s = 0; s < sphereGOs.size(); s++)
    {
        Gameobject* sphere = sphereGOs[s];
        for (int a = 0; a < aabbGOs.size(); a++)
        {
            Gameobject* aabb = aabbGOs[a];
            Vector3 sphereClosesPoint = sphere->position + ((aabb->position - sphere->position).normalized() * (sphere->scale.x/2.0f));
            if(Helper::isPointInsideAABB(aabb, sphereClosesPoint) == true)
            {
                // sphere is touching or is inside the aabb box

                // right face
                Vector3 contact;

                Vector3 topNormal = Vector3(0.0f, 1.0f, 0.0f);
                Vector3 frontNormal = Vector3(0.0f, 0.0f, 1.0f);
                Vector3 rightNormal = Vector3(1.0f, 0.0f, 0.0f);
                Vector3 bottomNormal = Vector3(0.0f, -1.0f, 0.0f);
                Vector3 backNormal = Vector3(0.0f, 0.0f, -1.0f);
                Vector3 leftNormal = Vector3(-1.0f, 0.0f, 0.0f);

                Vector3 topPlaneCoord = aabb->position;
                topPlaneCoord.y += aabb->scale.y / 2.0f;
                Vector3 frontPlaneCoord = aabb->position;
                frontPlaneCoord.z += aabb->scale.z / 2.0f;
                Vector3 rightPlaneCoord = aabb->position;
                rightPlaneCoord.x += aabb->scale.x / 2.0f;
                Vector3 bottomPlaneCoord = aabb->position;
                bottomPlaneCoord.y -= aabb->scale.y / 2.0f;
                Vector3 backPlaneCoord = aabb->position;
                backPlaneCoord.z -= aabb->scale.z / 2.0f;
                Vector3 leftPlaneCoord = aabb->position;
                leftPlaneCoord.x -= aabb->scale.x / 2.0f;


                bool isIntersectingTop = Helper::linePlaneIntersection(contact, (sphere->position-aabb->position), aabb->position, topNormal, topPlaneCoord);
                if (isIntersectingTop) {
                    bounceSphere(topNormal, sphere);
                    return;
                }
                bool isIntersectingFront = Helper::linePlaneIntersection(contact, (sphere->position - aabb->position), aabb->position, frontNormal, frontPlaneCoord);
                if (isIntersectingFront) {
                    bounceSphere(frontNormal, sphere);
                    return;
                }
                bool isIntersectingRight = Helper::linePlaneIntersection(contact, (sphere->position - aabb->position), aabb->position, rightNormal, rightPlaneCoord);
                if (isIntersectingRight) {
                    bounceSphere(rightNormal, sphere);
                    return;
                }
                bool isIntersectingBottom = Helper::linePlaneIntersection(contact, (sphere->position - aabb->position), aabb->position, bottomNormal, bottomPlaneCoord);
                if (isIntersectingBottom) {
                    bounceSphere(bottomNormal, sphere);
                    return;
                }
                bool isIntersectingBack = Helper::linePlaneIntersection(contact, (sphere->position - aabb->position), aabb->position, backNormal, backPlaneCoord);
                if (isIntersectingBack) {
                    bounceSphere(backNormal, sphere);
                    return;
                }
                bool isIntersectingLeft = Helper::linePlaneIntersection(contact, (sphere->position - aabb->position), aabb->position, leftNormal, leftPlaneCoord);
                if (isIntersectingLeft) {
                    bounceSphere(leftNormal, sphere);
                    return;
                }
            }
        }
    }
}

void checkSphereSphereCollisions() {
    for (int i = 0; i < sphereGOs.size(); i++)
    {
        for (int j = i + 1; j < sphereGOs.size(); j++)
        {
            float distance = sphereGOs[i]->position.distance(sphereGOs[j]->position);
            if (distance <= (sphereGOs[i]->scale.x / 2.0f + sphereGOs[j]->scale.x / 2.0f))
            {
                // balls collide
                Vector3 n_j = (sphereGOs[j]->position - sphereGOs[i]->position).normalized();
                Vector3 n_i = n_j * -1.0f;

                Vector3 Vn_j = n_j * (n_j.dot(sphereGOs[j]->velocity));
                sphereGOs[j]->velocity = sphereGOs[j]->velocity - (Vn_j * 2);

                Vector3 Vn_i = n_i * (n_i.dot(sphereGOs[i]->velocity));
                sphereGOs[i]->velocity = sphereGOs[i]->velocity - (Vn_i * 2);
            }
        }
    }
}

void ballIsOut(Gameobject* sphere)
{
    sphere->isActive = false;
    currentBallsOut++;
    if (currentBallsOut >= gameSettings.numOfBalls) {
        resetBalls();
        currentBallsOut = 0;
        currentLives--;
    }
    std::cout << "Current lives: " << currentLives << std::endl;
    if (currentLives <= 0) {
        std::cout << "Game Over!" << std::endl;
        std::cout << "Press ENTER to restart the game" << std::endl;
        isGameOver = true;
    }
}

void findClosestObjectInCircle(int &index, float &angle, Cylindrical3 speherePos, std::vector <Gameobject*> objects)
{
    int closestIndex = 0;
    float closestAngle = 1000.0f;
    for (int i = 0; i < objects.size(); i++)
    {
        Gameobject* object = objects[i];
        if (object->isActive == false) {
            continue;
        }
        if (object->position.y != 0.0f) { // skip those, that are not in corect level
            continue;
        }
        Cylindrical3 objectPos = object->position.toCylindrical();
        float angleDiff = Helper::angularDistance(Helper::toDegrees(speherePos.angle), Helper::toDegrees(objectPos.angle));
        if (angleDiff < closestAngle) {
            closestAngle = angleDiff;
            closestIndex = i;
        }
    }
    index = closestIndex;
    angle = closestAngle;
}

bool checkCollisionWithSides(Gameobject * sphere, Gameobject * object, float phiDeg, float objectRadius) 
{
    Cylindrical3 sphereCylPos = sphere->position.toCylindrical();
    Cylindrical3 objectCylPos = object->position.toCylindrical();


    Cylindrical3 A = Cylindrical3();
    Cylindrical3 B = Cylindrical3();
    if (Helper::angularDirection(sphereCylPos.angle, objectCylPos.angle) == 1) {
        A = Cylindrical3(objectRadius - 0.5f, objectCylPos.angle + Helper::toRadians(phiDeg), 0.0f);
        B = Cylindrical3(objectRadius + 0.5f, objectCylPos.angle + Helper::toRadians(phiDeg), 0.0f);
    }
    else {
        A = Cylindrical3(objectRadius - 0.5f, objectCylPos.angle - Helper::toRadians(phiDeg), 0.0f);
        B = Cylindrical3(objectRadius + 0.5f, objectCylPos.angle - Helper::toRadians(phiDeg), 0.0f);
    }
    Vector3 v = B.toCartesian() - A.toCartesian();
    Ray r = Ray(A.toCartesian(), v);
    Vector3 closestR = r.closest(sphere->position);
    if (closestR.toCylindrical().distance < objectRadius - 0.5f)
        closestR = A.toCartesian();
    if (closestR.toCylindrical().distance > objectRadius + 0.5f)
        closestR = B.toCartesian();
    float distance = closestR.distance(sphere->position);
    if (distance <= sphere->scale.x / 2.0f)
    {
        // ball touches edge of paddle
        Vector3 n = (closestR - sphere->position).normalized();
        bounceSphere(n, sphere);
        return true;
    }
    return false;
}

void checkCollisions()
{
    checkSphereAABBCollisions();
    checkSphereSphereCollisions();
    // sphere - paddles/walls collisions
    // check broad collisions
    for (int i = 0; i < sphereGOs.size(); i++)
    {
        Gameobject* sphere = sphereGOs[i];
        if (sphere->isActive == false) {
            continue;
        }
        Cylindrical3 speherePos = sphere->position.toCylindrical();

        // check outer boundary
        if (speherePos.distance > gameSettings.radius_border + (sphere->scale.x / 5))
        {
            ballIsOut(sphere);   
        }

        // from center to wall radius + half of wall diameter + shere radius
        else if (speherePos.distance <= gameSettings.radius_wall + (gameSettings.diameter_wall / 2.0f) + (sphere->scale.x / 2.0f)
            && speherePos.distance >= gameSettings.radius_wall - (gameSettings.diameter_wall / 2.0f) - (sphere->scale.x / 2.0f))
        {
            // check narrow wall collision
            if (wallGOs.size() <= 0)
                return;

            int closestIndex = -1;
            float closestAngle = -1;
            findClosestObjectInCircle(closestIndex, closestAngle, speherePos, wallGOs);

            // check closest wall collision
            Gameobject* wall = wallGOs[closestIndex];
            Cylindrical3 wallPos = wall->position.toCylindrical();
            if (closestAngle <= phi_wall) {
                // ball is colliding with the wall

                // get which direction of sphere with respect to paddle position (if the sphere in front or behind the wall)
                float dir = 1.0f;
                if (speherePos.distance < gameSettings.radius_wall) {
                    dir = -1.0f;
                }

                Vector3 n = sphere->position.normalized() * dir;
                bounceSphere(n, sphere);
                wallHit(wall, sphere);
            }
            else // closest angle > gameSettings.phi_walls so the ball might colliding with sides
            {
                bool isHit = checkCollisionWithSides(sphere, wall, phi_wall, gameSettings.radius_wall);
                if (isHit)
                {
                    wallHit(wall, sphere);
                }
            }

        }
        else if (speherePos.distance >= gameSettings.radius_paddles - (gameSettings.diameter_paddles / 2.0f) - (sphere->scale.x / 2.0f))
        {
            // check paddle collision narrow phase
            if (paddleGOs.size() <= 0)
                return;

            int closestIndex = -1;
            float closestAngle = -1;
            findClosestObjectInCircle(closestIndex, closestAngle, speherePos, paddleGOs);

            // check closest paddle collision
            Gameobject* paddle = paddleGOs[closestIndex];
            Cylindrical3 paddlePos = paddle->position.toCylindrical();
            if (closestAngle <= gameSettings.phi_paddles) {
                // ball is colliding with the paddle's inside or outside face
                Vector3 n = sphere->position.normalized() * -1.0f; // the collision will always be off inside face
                bounceSphere(n, sphere);
            }
            else // closest angle > gameSettings.phi_paddles so the ball might colliding with sides
            { 
                checkCollisionWithSides(sphere, paddle, gameSettings.phi_paddles, gameSettings.radius_paddles);
            }
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

void createWalls(Shader shaderProgram)
{
    for (int f = 0; f < gameSettings.numOfWallFloors; f++)
    {
        for (int s = 0; s < gameSettings.numOfWallSegments; s++)
        //for (int s = 0; s < 1; s++)
        {
            // create
            Gameobject* wall = new Gameobject(shaderProgram, &wallMesh);
            // find poition on the circle
            wall->position = Vector3(gameSettings.radius_wall, f, 0.0f);
            float angle = (phi_wall * 2.0f) * float(s);
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, Helper::toRadians(angle), 0.0f);
            wall->position = rotCenter * wall->position;
            if ((f + s) % 2 == 1) {
                wall->color = Vector3(65.0f / 255.0f, 171.0f / 255.0f, 51.0f / 255.0f);
            }
            else {
                wall->color = Vector3(207.0f / 255.0f, 149.0f / 255.0f, 14.0f / 255.0f);
            }
            if (f > 0) {
                wall->transparency = 0.5;
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
}

void updateSpheresRotationWithPaddles(float dt) {
    for (int i = 0; i < sphereGOs.size(); i++)
    {
        Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, paddleRotation * Helper::toRadians(gameSettings.paddleRotationSpeed * dt), 0.0f);
        sphereGOs[i]->position = rotCenter * sphereGOs[i]->position;
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
    gameSettings.ballSpeed = 5.0f;
    gameSettings.paddleRotationSpeed = 90.0f; // degrees per second
    gameSettings.maxLives = 3;
    gameSettings.numOfWallSegments = 3;
    gameSettings.numOfWallFloors = 1;
    gameSettings.radius_wall = 5.0f;
    gameSettings.betweenLevelsTime = 2.0f;
    gameSettings.numOfBalls = 1;

    currentBallSpeed = gameSettings.ballSpeed;
    currentLives = gameSettings.maxLives;
    phi_wall = (360.0f / float(gameSettings.numOfWallSegments)) / 2.0f;

    loadMeshes();

    std::cout << "Game starts!" << std::endl;
    std::cout << "Current lives: " << currentLives << std::endl;

    GLFWwindow* window = openWindow();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader phongShader("shaders/phong.vs", "shaders/phong.fs");
    Shader ambientShader("shaders/ambient.vs", "shaders/ambient.fs");
    Shader textureShader("shaders/texture.vs", "shaders/texture.fs");

    for (int i = 0; i < gameSettings.numOfBalls; i++)
    {
        Gameobject * sphere = new Gameobject(phongShader, &sphereMesh);
        sphere->position = Vector3(float(i), 0.0f, 0.0f);
        sphere->color = Vector3(170.0f / 255.0f, 174.0f / 255.0f, 181.0f / 255.0f);
        GOs.push_back(sphere);
        sphereGOs.push_back(sphere);
    }
    

    Gameobject * floor = new Gameobject(textureShader, &circleMesh, "sand.jpg");
    floor->position = Vector3(0.0f, -0.5f, 0.0f);
    //floor->rotation = Vector3(0.0f, 0.0f, Helper::toRadians(180));
    GOs.push_back(floor);

    //Gameobject * triangle = new Gameobject(textureShader, &triangleMesh, true);
    //triangle->scale = Vector3(5.0f, 5.0f, 5.0f);
    //GOs.push_back(triangle);

    //Gameobject * cube = new Gameobject(phongShader, &cubeMesh);
    //cube->scale = Vector3(5.0f, 1.0f, 7.0f);
    //cube->position = Vector3(0.0f, 0.0f, 0.0f);
    //aabbGOs.push_back(cube);
    //GOs.push_back(cube);


    createPaddles(phongShader);
    createWalls(phongShader);

    resetBalls();

    auto lastTick = std::chrono::high_resolution_clock::now();
    double dt;

    // backface culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        auto now = std::chrono::high_resolution_clock::now();

        dt = std::chrono::duration<double, std::milli>(now - lastTick).count()/1000.0f;
        lastTick = now;

        if(newLevelTimer == true){
            timer += dt;
            if (timer >= gameSettings.betweenLevelsTime) {
                resetWall();
                newLevelTimer = false;
            }
        }

        // input
        // -----
        processInput(window);

        checkCollisions();

        //sphere2->rotation = sphere2->rotation + Vector3(0.0f, Helper::toRadians(.5f * dt), 0.0f);

        if (isBallReadyToFire == true) 
        {
            updateSpheresRotationWithPaddles(dt);
        }

        for (int i = 0; i < paddleGOs.size(); i++)
        {
            Matrix4 rotCenter = Matrix4::rotationMatrix(0.0f, paddleRotation * Helper::toRadians(gameSettings.paddleRotationSpeed * dt), 0.0f);
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
    GOs.clear();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        closeApplication();
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
        fireBall(false);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        cameraMode = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        cameraMode = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        cameraMode = 3;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        cameraMode = 4;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        cameraMode = 5;
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