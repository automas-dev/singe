#version 330 core

#include "util.glsl"
#include "light.glsl"
#include "gamma.glsl"

in vec2 FragTex;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool hdr;
uniform int mode;

vec3 FragPos;
vec3 FragNorm;
vec3 FragAlbedo;
float FragSpecular;

void main() {
    FragPos = texture(gPosition, FragTex).rgb;
    FragNorm = texture(gNormal, FragTex).rgb;
    FragAlbedo = texture(gAlbedo, FragTex).rgb;
    FragSpecular = texture(gSpecular, FragTex).r;

    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 color = lightAmbient(FragAlbedo);
    color += lightDirectional(FragPos, FragAlbedo, FragSpecular, lightPos, normal, viewDir);

    switch (mode) {
        case 0:
            color = FragPos;
            break;
        case 1:
            color = FragNorm;
            break;
        case 2:
            color = FragAlbedo;
            break;
        case 3:
            color = vec3(FragSpecular);
            break;
    }

    if (hdr)
        color = tonemap(color);
    FragColor = vec4(color, 1.0);
}
