#version 330 core

uniform sampler2D tex;

in vec4 ourColor;
in vec2 ourTex;

out vec4 FragColor;

void main()
{
    FragColor = ourColor * texture(tex, ourTex);
}
