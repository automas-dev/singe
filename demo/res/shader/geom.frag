#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform sampler2D tex;
uniform float specular;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(FragNorm);
    gAlbedo.rgb = texture(tex, FragTex).rgb;
    gAlbedo.a = specular;
}

