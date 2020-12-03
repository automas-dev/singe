#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
    float alpha;

    sampler2D texture;
};

uniform Material material;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

uniform sampler2D tex;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

void main()
{
    gPosition = FragPos;
    gNormal = normalize(FragNorm);
    gAlbedo.rgb = texture(tex, FragTex).rgb;
    gAlbedo.a = material.shininess;
}

