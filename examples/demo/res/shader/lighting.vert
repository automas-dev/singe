#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 2) in vec2 aTex;

out vec2 FragTex;

uniform mat4 vp;
uniform mat4 model;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    FragTex = aTex;
}

