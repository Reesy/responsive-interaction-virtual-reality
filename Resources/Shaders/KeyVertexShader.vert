#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
out vec4 colColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 interactionColor;
uniform float collide;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
   // colColor = vec4(1000.0f, 1.5f, 1.0f, 1.0f);
    colColor = vec4(collide, 1.5f, 1.0f, 1.0f);
}