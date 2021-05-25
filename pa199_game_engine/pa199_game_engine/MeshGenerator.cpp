#include "MeshGenerator.h"

// http://www.songho.ca/opengl/gl_sphere.html
Mesh MeshGenerator::Sphere(float radius)
{
    // constants
    const float PI = 3.1415926f;
    const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
    const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

    std::vector<unsigned int> indexes;
    std::vector<float> vertices(12 * 3);    // array of 12 vertices (x,y,z)
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
            indexes.push_back(i2_prev / 3);
            indexes.push_back(i2 / 3);
            i2_prev_prev = i2_prev;
        }
        if (i1_prev_prev != -1) {
            indexes.push_back(i1_prev/3);
            indexes.push_back(i1/3);
            indexes.push_back(i2_prev /3);
        }
        if (i2_prev_prev != -1) {
            indexes.push_back(i1 / 3);
            indexes.push_back(i2 / 3);
            indexes.push_back(i2_prev / 3);
        }
        
        i1_prev = i1;
        i2_prev = i2;
    }

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

    std::size_t i;
    std::size_t count = vertices.size();
    for (i = 0; i < count; i += 3)
    {
        verticesV3.push_back(Vector3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }


    return Mesh(verticesV3, indexes);
}

//std::vector<float> MeshGenerator::SphereGLTriangles(float radius)
//{
//    std::vector<Vector3> verticesV3 = Sphere(radius);
//    std::vector<Vector3> vertexesTriangles;
//
//    vertexesTriangles.push_back(verticesV3[0]);
//    vertexesTriangles.push_back(verticesV3[1]);
//    vertexesTriangles.push_back(verticesV3[2]);
//
//    vertexesTriangles.push_back(verticesV3[0]);
//    vertexesTriangles.push_back(verticesV3[1]);
//    vertexesTriangles.push_back(verticesV3[2]);
//
//}
