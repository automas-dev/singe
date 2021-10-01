#version 330 core

out vec4 FragColor;

uniform sampler2D gTexture;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;
flat in int FragInstance;

void main() {
    FragColor = texture(gTexture, FragTex);
}
