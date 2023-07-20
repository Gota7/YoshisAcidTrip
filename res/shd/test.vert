#version 330

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// Out to fragment.
out vec3 Color;

// Main func.
void main()
{
    gl_Position = vec4(aPos, 1.0);
    Color = aColor;
}