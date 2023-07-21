#version 330

out vec4 FragColor;
in vec2 UV;
uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, UV);
    // FragColor = vec4(1, 0, 0, 1);
}