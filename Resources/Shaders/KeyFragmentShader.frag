#version 330 core

in vec2 TexCoords;
in vec4 colColor;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
   // color = vec4(texture(texture_diffuse1, TexCoords) * vec4(colColor, 1.0f));
     color = vec4(texture(texture_diffuse1, TexCoords) * colColor);
   // color = vec4(texture(texture_diffuse1, TexCoords));
    
    // color = vec4(texture(texture_diffuse1, TexCoords) * vec4(1000.0f, 1.5f, 1.0f, 1.0f));
}