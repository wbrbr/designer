#version 330 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    float val = texture(tex, UV).r;
    FragColor = vec4(val, val, val, 1.0);
}
