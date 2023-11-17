#version 330

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

// Out to fragment.
out vec4 Color;
out vec2 UV;

// Main func.
void main()
{
    gl_Position = vec4(aPos, 1.0);
    Color = aColor;
    UV = aUV;
}