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
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 


vec3 CalculateDiffuse(vec3 lightDirection, vec3 normal)
{
    // difuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    return diffuse;
}

vec3 CalculateSpecular(vec3 lightDirection, vec3 normal)
{

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    return specular;
}


void main()
{

    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-mat3(view) * light.direction);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // difuse
    vec3 diffuse = CalculateDiffuse(lightDir, norm);

    // specular
    vec3 specular = CalculateSpecular(lightDir, norm);


    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}