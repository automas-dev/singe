#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 1) in vec2 aTex;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 FragTex;

uniform mat4 vp;
uniform mat4 model;

void main()
{
    gl_Position = vp * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    FragNorm = aNorm;
    FragTex = aTex;
}
