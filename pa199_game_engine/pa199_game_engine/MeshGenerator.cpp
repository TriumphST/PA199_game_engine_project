#include "MeshGenerator.h"

Mesh MeshGenerator::Triangle()
{
    std::vector<Vector3> triangleVertices = {
    Vector3(-0.5f, -0.5f, 0.0f),
    Vector3(0.5f, -0.5f, 0.0f),
    Vector3(0.0f,  0.5f, 0.0f)
    };

    std::vector<unsigned int> triangleIndexes = {
        0, 1, 2
    };
    return Mesh(triangleVertices, triangleIndexes);
}

Mesh MeshGenerator::Squere()
{
    std::vector<Vector3> squereVertices = {
        Vector3(-0.5f, -0.5f, 0.0f),   //0
        Vector3(0.5f, -0.5f, 0.0f),    //1
        Vector3(0.5f,  0.5f, 0.0f),   //2
        Vector3(-0.5f, -0.5f, 0.0f),   //0 >3
        Vector3(0.5f,  0.5f, 0.0f),   //2 >4
        Vector3(-0.5f,  0.5f, 0.0f),    //3 >5
    };

    std::vector<unsigned int> squereIndexes = {
            0,1,2,
            3,4,5
    };

    std::vector<Vector3> squereNormals = {
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
    };
    return Mesh(squereVertices, squereIndexes, squereNormals);
}

Mesh MeshGenerator::Cube()
{
    Vector3 front_left_down = Vector3(-0.5f, -0.5f, 0.5f); //0
    Vector3 front_right_down = Vector3(0.5f, -0.5f, 0.5f); //1
    Vector3 front_right_up = Vector3(0.5f, 0.5f, 0.5f);    //2
    Vector3 front_left_up = Vector3(-0.5f, 0.5f, 0.5f);    //3
    Vector3 back_left_down = Vector3(-0.5f, -0.5f, -0.5f); //4
    Vector3 back_right_down = Vector3(0.5f, -0.5f, -0.5f); //5
    Vector3 back_right_up = Vector3(0.5f, 0.5f, -0.5f);    //6
    Vector3 back_left_up = Vector3(-0.5f, 0.5f, -0.5f);    //7

    std::vector<Vector3> cubeVertices = {
        // front face trinagle 1
        front_left_down,
        front_right_down,
        front_right_up,

        // front face trinagle 2
        front_left_down,
        front_right_up,
        front_left_up,

        // back face trinagle 1
        back_left_down,
        back_right_down,
        back_right_up,

        // back face trinagle 2
        back_left_down,
        back_right_up,
        back_left_up,

        // left face trinagle 1
        back_left_down,
        front_left_down,
        front_left_up,

        // left face trinagle 2
        back_left_down,
        front_left_up,
        front_right_up,

        // right face trinagle 1
        front_right_down,
        back_right_down,
        back_right_up,

        // right face trinagle 2
        front_right_down,
        back_right_up,
        front_right_up,

        // up face trinagle 1
        front_left_up,
        front_right_up,
        back_right_up,

        // up face trinagle 2
        front_left_up,
        back_right_up,
        back_left_up,

        // down face trinagle 1
        front_left_down,
        front_right_down,
        back_right_down,

        // down face trinagle 2
        front_left_down,
        back_right_down,
        back_left_down,

    };

    std::vector<unsigned int> cubeIndexes = {
        0, 1, 2,
        3, 4, 5,

        6, 7, 8,
        9, 10, 11,

        12, 13, 14,
        15, 16, 17,

        18, 19, 20,
        21, 22, 23,

        24, 25, 26,
        27, 28, 29,

        30, 31, 32,
        33, 34, 35
    };

    std::vector<Vector3> cubeNormals = {
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),
        Vector3(0.0f,  0.0f, 1.0f),

        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),
        Vector3(0.0f,  0.0f, -1.0f),

        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),
        Vector3(-1.0f,  0.0f,  0.0f),

        Vector3(1.0f,  0.0f,  0.0f),
        Vector3(1.0f,  0.0f,  0.0f),
        Vector3(1.0f,  0.0f,  0.0f),
        Vector3(1.0f,  0.0f,  0.0f),
        Vector3(1.0f,  0.0f,  0.0f),
        Vector3(1.0f,  0.0f,  0.0f),

        Vector3(0.0f,  1.0f,  0.0f),
        Vector3(0.0f,  1.0f,  0.0f),
        Vector3(0.0f,  1.0f,  0.0f),
        Vector3(0.0f,  1.0f,  0.0f),
        Vector3(0.0f,  1.0f,  0.0f),
        Vector3(0.0f,  1.0f,  0.0f),

        Vector3(0.0f, -1.0f,  0.0f),
        Vector3(0.0f, -1.0f,  0.0f),
        Vector3(0.0f, -1.0f,  0.0f),
        Vector3(0.0f, -1.0f,  0.0f),
        Vector3(0.0f, -1.0f,  0.0f),
        Vector3(0.0f, -1.0f,  0.0f),
    };

    return Mesh(cubeVertices, cubeIndexes, cubeNormals);
}

// http://www.songho.ca/opengl/gl_sphere.html
Mesh MeshGenerator::Sphere(float radius)
{
    // constants
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    std::vector<unsigned int> indexes;
    std::vector<float> vertices(12 * 3);    // array of 12 vertices (x,y,z)
    std::vector<float> normals(12 * 3);    // array of 12 normals (x,y,z)
    int i1, i2;                             // indices
    float z, xy;                            // coords
    float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 1st row
    float hAngle2 = -PI / 2;                // start from -90 deg at 2nd row

    // the first top vertex at (0, 0, r)
    vertices[0] = 0;
    vertices[1] = 0;
    vertices[2] = radius;

    int i1_prev_prev = -1;
    int i2_prev_prev = -1;
    int i1_prev = -1;
    int i2_prev = -1;

    // compute 10 vertices at 1st and 2nd rows
    for (int i = 1; i <= 5; ++i)
    {
        i1 = i * 3;         // index for 1st row
        i2 = (i + 5) * 3;   // index for 2nd row

        z = radius * sinf(V_ANGLE);            // elevaton
        xy = radius * cosf(V_ANGLE);            // length on XY plane

        vertices[i1] = xy * cosf(hAngle1);      // x
        vertices[i2] = xy * cosf(hAngle2);
        vertices[i1 + 1] = xy * sinf(hAngle1);  // y
        vertices[i2 + 1] = xy * sinf(hAngle2);
        vertices[i1 + 2] = z;                   // z
        vertices[i2 + 2] = -z;

        // next horizontal angles
        hAngle1 += H_ANGLE;
        hAngle2 += H_ANGLE;

        if(i1_prev != -1){
            // top
            indexes.push_back(0);
            indexes.push_back(i1_prev/3);
            indexes.push_back(i1/3);
            i1_prev_prev = i1_prev;
        }
        if (i2_prev != -1) {
            // bottom
            indexes.push_back(11);
            indexes.push_back(i2 / 3);
            indexes.push_back(i2_prev / 3);
            i2_prev_prev = i2_prev;
        }
        if (i1_prev_prev != -1) {
            indexes.push_back(i1_prev/3);
            indexes.push_back(i2_prev / 3);
            indexes.push_back(i1/3);
        }
        if (i2_prev_prev != -1) {
            indexes.push_back(i1 / 3);
            indexes.push_back(i2_prev / 3);
            indexes.push_back(i2 / 3);
        }
        
        i1_prev = i1;
        i2_prev = i2;
    }

    // middle last 2 triangles
    indexes.push_back(i1_prev / 3);
    indexes.push_back(i2_prev / 3);
    indexes.push_back(1);

    indexes.push_back(1);
    indexes.push_back(i2_prev / 3);
    indexes.push_back(6);

    // top last triangle
    indexes.push_back(0);
    indexes.push_back(i1_prev / 3);
    indexes.push_back(1);

    // bottom last triangle
    indexes.push_back(11);
    indexes.push_back(10);
    indexes.push_back(6);

    // the last bottom vertex at (0, 0, -r)
    i1 = 11 * 3;
    vertices[i1] = 0;
    vertices[i1 + 1] = 0;
    vertices[i1 + 2] = -radius;

    std::vector<Vector3> verticesV3;
    std::vector<Vector3> normalsV3;

    std::size_t i;
    std::size_t count = vertices.size();
    for (i = 0; i < count; i += 3)
    {
        // swap Y and Z to match our coordinate system
        verticesV3.push_back(Vector3(vertices[i], vertices[i + 2], vertices[i + 1]));
        normalsV3.push_back(verticesV3[i/3].normalized());
    }

    return Mesh(verticesV3, indexes, normalsV3); // in this case, normals === vertexes
}

Mesh MeshGenerator::Paddle(float phi, int numOfLineVertexes, float distance)
{
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<unsigned int> indexes;

    Vector3 topNormal = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 frontNormal = Vector3(0.0f, 0.0f, 1.0f);
    Vector3 rightNormal = Vector3(1.0f, 0.0f, 0.0f);

    Vector3 bottomNormal = Vector3(0.0f, -1.0f, 0.0f);
    Vector3 backNormal = Vector3(0.0f, 0.0f, -1.0f);
    Vector3 leftNormal = Vector3(-1.0f, 0.0f, 0.0f);

    float angleBetweenVertexes = (phi * 2) / numOfLineVertexes;

    // top near row
    Cylindrical3 vertexPos = Cylindrical3(distance-0.5f, -Helper::toRadians(angleBetweenVertexes * (numOfLineVertexes / 2)), 0.5f);
    for (int i = 0; i < numOfLineVertexes; i++)
    {
        vertices.push_back(vertexPos.toCartesian());
        vertexPos.angle += Helper::toRadians(angleBetweenVertexes);
    }
    vertices.push_back(vertexPos.toCartesian());

    // top far row
    vertexPos = Cylindrical3(distance+0.5f, -Helper::toRadians(angleBetweenVertexes * (numOfLineVertexes / 2)), 0.5f);
    for (int i = 0; i < numOfLineVertexes; i++)
    {
        vertices.push_back(vertexPos.toCartesian());
        vertexPos.angle += Helper::toRadians(angleBetweenVertexes);
    }
    vertices.push_back(vertexPos.toCartesian());

    // bottom near row
    vertexPos = Cylindrical3(distance - 0.5f, -Helper::toRadians(angleBetweenVertexes * (numOfLineVertexes / 2)), -0.5f);
    for (int i = 0; i < numOfLineVertexes; i++)
    {
        vertices.push_back(vertexPos.toCartesian());
        vertexPos.angle += Helper::toRadians(angleBetweenVertexes);
    }
    vertices.push_back(vertexPos.toCartesian());

    // bottom far row
    vertexPos = Cylindrical3(distance + 0.5f, -Helper::toRadians(angleBetweenVertexes * (numOfLineVertexes / 2)), -0.5f);
    for (int i = 0; i < numOfLineVertexes; i++)
    {
        vertices.push_back(vertexPos.toCartesian());
        vertexPos.angle += Helper::toRadians(angleBetweenVertexes);
    }
    vertices.push_back(vertexPos.toCartesian());

    int bottomStartIndex = (numOfLineVertexes * 2) + 2;
    for (int i = 0; i < numOfLineVertexes; i++)
    {
        // top
        indexes.push_back(i + 0);
        indexes.push_back(i + 1);
        indexes.push_back(i + 1 + numOfLineVertexes); // 11

        indexes.push_back(i + 1);
        indexes.push_back(i + 2 + numOfLineVertexes); // 12
        indexes.push_back(i + 1 + numOfLineVertexes); // 11

        normals.push_back(topNormal);
        normals.push_back(topNormal);
        normals.push_back(topNormal);
        normals.push_back(topNormal);
        normals.push_back(topNormal);
        normals.push_back(topNormal);

        // bottom
        indexes.push_back(bottomStartIndex + i + 0); //22
        indexes.push_back(bottomStartIndex + i + 1 + numOfLineVertexes);
        indexes.push_back(bottomStartIndex + i + 1); //23

        indexes.push_back(bottomStartIndex + i + 1);
        indexes.push_back(bottomStartIndex + i + 1 + numOfLineVertexes);
        indexes.push_back(bottomStartIndex + i + 2 + numOfLineVertexes);

        normals.push_back(bottomNormal);
        normals.push_back(bottomNormal);
        normals.push_back(bottomNormal);
        normals.push_back(bottomNormal);
        normals.push_back(bottomNormal);
        normals.push_back(bottomNormal);

        // front
        indexes.push_back(i + 0);
        indexes.push_back(bottomStartIndex + i + 0);
        indexes.push_back(i + 1);

        indexes.push_back(i + 1);
        indexes.push_back(bottomStartIndex + i + 0);
        indexes.push_back(bottomStartIndex + i + 1);

        normals.push_back(leftNormal);
        normals.push_back(leftNormal);
        normals.push_back(leftNormal);
        normals.push_back(leftNormal);
        normals.push_back(leftNormal);
        normals.push_back(leftNormal);

        // back
        indexes.push_back(i + 1 + numOfLineVertexes);
        indexes.push_back(i + 2 + numOfLineVertexes);
        indexes.push_back(bottomStartIndex + i + 1 + numOfLineVertexes);

        indexes.push_back(i + 2 + numOfLineVertexes);
        indexes.push_back(bottomStartIndex + i + 2 + numOfLineVertexes);
        indexes.push_back(bottomStartIndex + i + 1 + numOfLineVertexes);

        normals.push_back(rightNormal);
        normals.push_back(rightNormal);
        normals.push_back(rightNormal);
        normals.push_back(rightNormal);
        normals.push_back(rightNormal);
        normals.push_back(rightNormal);

    }
    // first side
    indexes.push_back(0);
    indexes.push_back(1 + numOfLineVertexes);
    indexes.push_back(bottomStartIndex + 0);

    indexes.push_back(1 + numOfLineVertexes);
    indexes.push_back(bottomStartIndex + 1 + numOfLineVertexes);
    indexes.push_back(bottomStartIndex + 0);

    normals.push_back(frontNormal);
    normals.push_back(frontNormal);
    normals.push_back(frontNormal);
    normals.push_back(frontNormal);
    normals.push_back(frontNormal);
    normals.push_back(frontNormal);

    // last side
    indexes.push_back(numOfLineVertexes); //10
    indexes.push_back(bottomStartIndex + numOfLineVertexes); //32
    indexes.push_back(bottomStartIndex - 1); //21

    indexes.push_back(bottomStartIndex + numOfLineVertexes); //32
    indexes.push_back(bottomStartIndex + 2* numOfLineVertexes + 1); //43
    indexes.push_back(bottomStartIndex - 1); //21

    normals.push_back(backNormal);
    normals.push_back(backNormal);
    normals.push_back(backNormal);
    normals.push_back(backNormal);
    normals.push_back(backNormal);
    normals.push_back(backNormal);

    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].x -= distance;
    }

    std::vector<Vector3> vertexedPerNormal(indexes.size());
    std::vector<unsigned int> remapedIndexes(indexes.size());
    // remap revtexes becouse of indexing
    for (int i = 0; i < indexes.size(); i++)
    {
        vertexedPerNormal[i] = vertices[indexes[i]];
        remapedIndexes[i] = i;
    }

    return Mesh(vertexedPerNormal, remapedIndexes, normals);
}