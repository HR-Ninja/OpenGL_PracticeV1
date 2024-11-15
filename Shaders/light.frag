#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos; 
in vec2 TexCoords;

uniform mat4 view;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 


void main()
{
    
    vec3 lightViewPos = vec3(view * vec4(light.position, 1.0));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // difuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightViewPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));


    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  


    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}