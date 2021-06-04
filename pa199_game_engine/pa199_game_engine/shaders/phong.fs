#version 330 core
in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 color;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.5;
    vec3 lightPos = vec3(2.0,2.0,2.0);
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 objectColor = color; //vec3(1.0, 0.5, 0.2);
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}