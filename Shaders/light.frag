#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; 

uniform mat4 model;
uniform mat4 view;


uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    
    vec3 lightViewPos = vec3(view * model * vec4(lightPos, 1.0));

    float ambientStrength = 0.15f;
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.5f;

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightViewPos - FragPos); 
    vec3 viewDir = normalize(-FragPos);


    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 reflectDir = reflect(-lightDir, norm); 

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}