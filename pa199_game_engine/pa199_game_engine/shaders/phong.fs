#version 330 core
out vec4 FragColor;
void main()
{
    float ambientStrength = 0.5;
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 objectColor = vec3(1.0, 0.5, 0.2);
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}