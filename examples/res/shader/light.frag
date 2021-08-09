#version 330 core

in vec2 FragTex;

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform bool hdr;

vec3 FragPos;
vec3 FragNorm;
vec3 FragAlbedo;
float FragSpecular;

vec3 ambient = vec3(0.3, 0.3, 0.3);
vec3 diffuse = vec3(0.8, 0.8, 0.8);
vec3 specular = vec3(0.8, 0.8, 0.8);

#include "util.glsl"

vec3 calcLight(vec3 light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), FragSpecular);

    // combine results
    vec3 diffuse = diffuse * FragAlbedo * diff;
    vec3 specular = specular * FragSpecular * spec;

    return (diffuse + specular);
}

vec3 tonemap(vec3 color)
{
    const float gamma = 2.2;
    vec3 mapped = color / (color + vec3(1.0));
    return pow(mapped, vec3(1.0 / gamma));
}

void main()
{
    FragPos = texture(gPosition, FragTex).rgb;
    FragNorm = texture(gNormal, FragTex).rgb;
    FragAlbedo = texture(gAlbedo, FragTex).rgb;
    FragSpecular = texture(gSpecular, FragTex).r;

    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 color = calcAmbient();
    color += calcLight(lightPos, normal, viewDir);

    if (hdr)
        color = tonemap(color);
    FragColor = vec4(color, 1.0);
}
