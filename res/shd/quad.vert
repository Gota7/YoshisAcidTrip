#version 330

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Out to fragment.
out vec3 FragPos;
out vec2 TexCoords;

// Main func.
void main()
{
    FragPos = aPos;
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}