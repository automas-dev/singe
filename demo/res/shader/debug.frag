#version 330 core

in vec2 FragTex;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

void main()
{
    vec3 pos = texture(gPosition, FragTex).rgb;
    vec3 color = texture(gAlbedo, FragTex).rgb * pos;
    FragColor = vec4(color / (abs(color) + vec3(1.0)), 1.0);
}

