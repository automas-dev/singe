#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in mat4 aModel;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 FragTex;
flat out int FragInstance; 

uniform mat4 mvp;

void main() {
    gl_Position = mvp * aModel * vec4(aPos, 1.0);
    FragPos = vec3(aModel * vec4(aPos, 1.0));
    FragNorm = aNorm;
    FragTex = aTex;
    FragInstance = gl_InstanceID;
}
