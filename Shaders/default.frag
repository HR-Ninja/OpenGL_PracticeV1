#version 460 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform float uAlpha = 1.0f;

void main()
{
    vec4 color = texture(texture1, TexCoord);
    color.w = uAlpha;
    FragColor = color;
}