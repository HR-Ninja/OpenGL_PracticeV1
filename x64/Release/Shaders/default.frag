#version 460 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float uAlpha = 1.0f;

void main()
{
    vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.4f);
    color.w = uAlpha;
    FragColor = color;
}