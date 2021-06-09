#version 330 core
in vec3 FragPos;  
in vec3 Normal;  
in vec3 NormalRaw;

uniform vec3 color;
uniform float transparency;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    //FragColor = vec4(NormalRaw, 1.0);
    //FragColor = vec4(transparency, 0.0, 0.0, 1.0);
    //return;
    
    vec3 lightPos = vec3(15.0,20.0,20.0);
    vec3 lightColor = vec3(1.0,1.0,1.0);
    vec3 objectColor = color; //vec3(1.0, 0.5, 0.2);

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.8;
    float shininess = 16;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, transparency);
}