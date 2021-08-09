#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out float gSpecular;

uniform sampler2D tex;
uniform float specular;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(FragNorm);
    gAlbedo = texture(tex, FragTex).rgb;
    gSpecular = specular;
}

