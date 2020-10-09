#version 330 core

#define N_MAX_LIGHTS 256

const float ambientStrength = 1.0;
const float diffuseStrength = 1.0;
const float specularStrength = 1.0;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
    float alpha;

    sampler2D texture;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    uint type;
};

uniform Material material;

uniform Light lights[N_MAX_LIGHTS];
uniform int nLights;

uniform vec3 viewPos;

out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

vec3 calcLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 texColor = vec3(texture(material.texture, FragTex));

    vec3 ambient = light.ambient * material.ambient * texColor * ambientStrength;
    vec3 diffuse = light.diffuse * material.diffuse * texColor * diff * diffuseStrength;
    vec3 specular = light.specular * material.specular * spec * diffuseStrength;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 color = vec3(0.0);
    for (int i = 0; i < nLights; i++) {
        color += calcLight(lights[i], normal, viewDir);
    }

    FragColor = vec4(color, material.alpha);
}
