#version 330

out vec4 FragColor;
in vec4 Color;
in vec2 UV;
uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, UV) * Color;
}