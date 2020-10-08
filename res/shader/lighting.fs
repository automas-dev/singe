#version 330 core

in vec3 FragPos;
in vec3 FragNorm;

out vec4 FragColor;

const float ambientStrength = 1.0;
const float diffuseStrength = 1.0;
const float specularStrength = 1.0;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

uniform float specExp;
uniform float alpha;

void main()
{
    vec3 color = vec3(0.0);
    color += ambient * ambientStrength;

    vec3 norm = normalize(FragNorm);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    color += diffuseStrength * diff * diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);
    color += specularStrength * spec * specular;

    FragColor = vec4(color / 3.0, alpha);
}
