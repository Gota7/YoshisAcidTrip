#version 330

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Out to fragment.
out vec2 TexCoords;

// Main func.
void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}