#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 FragTex;

uniform mat4 mvp;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(aPos, 1.0);
    FragPos = (model * vec4(aPos, 1.0)).xyz;
    FragNorm = aNorm;
    FragTex = aTex;
}

