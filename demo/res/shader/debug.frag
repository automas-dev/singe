#version 330 core

in vec2 FragTex;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

uniform int show = 0;
uniform int tonemap = 1;

void main()
{
    vec3 pos = texture(gPosition, FragTex).rgb;
    vec3 normal = texture(gNormal, FragTex).rgb;
    vec3 albedo = texture(gAlbedo, FragTex).rgb;
    vec3 color;
    switch (show) {
        default:
            color = albedo * pos;
            break;
        case 1:
            color = pos;
            break;
        case 2:
            color = normal;
            break;
        case 3:
            color = albedo;
            break;
    }
    if (tonemap != 0)
        FragColor = vec4(color / (abs(color) + vec3(1.0)), 1.0);
    else
        FragColor = vec4(color, 1.0);
}

