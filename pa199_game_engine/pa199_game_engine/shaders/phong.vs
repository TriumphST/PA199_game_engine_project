#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos; 
out vec3 Normal;
out vec3 NormalRaw;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    //lighting calculations are in world space, we need vertex position in world space, multiplying the vertex position by model matrix only
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    NormalRaw = aNormal;
    // also tranform normals to word space so scaling does not affect them
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}