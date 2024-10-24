#version 460 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;


    FragColor = vec4(ambient * objectColor, 1.0);
}