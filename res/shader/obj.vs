#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNorm;

out vec3 ourPos;
out vec2 ourTex;
out vec3 ourNorm;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

uniform mat4 mvp;

void main()
{
    ourPos = mvp * vec4(aPos, 1.0);
    gl_Position = ourPos;
    ourTex = aTex;
    ourNorm = aNorm;
}
