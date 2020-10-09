#version 330 core

#define N_MAX_LIGHTS 256

#define LIGHT_TYPE_DIR 0u
#define LIGHT_TYPE_POINT 1u
#define LIGHT_TYPE_SPOT 2u

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

struct Point {
    float constant;
    float linear;
    float quadratic;
};

struct Spot {
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;
    uint type;

    Point point;
    Spot spot;
};

uniform Material material;

uniform Light lights[N_MAX_LIGHTS];
uniform uint nLights;

uniform vec3 viewPos;

out vec4 FragColor;

in vec3 FragPos;
in vec3 FragNorm;
in vec2 FragTex;

vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 texColor = vec3(texture(material.texture, FragTex));
    vec3 ambient = light.ambient * material.ambient * texColor * ambientStrength;
    vec3 diffuse = light.diffuse * material.diffuse * texColor * diff * diffuseStrength;
    vec3 specular = light.specular * material.specular * spec * diffuseStrength;

    return (ambient + diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 texColor = vec3(texture(material.texture, FragTex));
    vec3 ambient = light.ambient * material.ambient * texColor * ambientStrength;
    vec3 diffuse = light.diffuse * material.diffuse * texColor * diff * diffuseStrength;
    vec3 specular = light.specular * material.specular * spec * diffuseStrength;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.point.constant + light.point.linear * distance + light.point.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 texColor = vec3(texture(material.texture, FragTex));
    vec3 ambient = light.ambient * material.ambient * texColor * ambientStrength;
    vec3 diffuse = light.diffuse * material.diffuse * texColor * diff * diffuseStrength;
    vec3 specular = light.specular * material.specular * spec * diffuseStrength;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.point.constant + light.point.linear * distance + light.point.quadratic * (distance * distance));    

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.spot.cutOff - light.spot.outerCutOff;
    float intensity = clamp((theta - light.spot.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

vec3 calcLight(Light light, vec3 normal, vec3 viewDir) {
    if (light.type == LIGHT_TYPE_DIR)
        return calcDirLight(light, normal, viewDir);
    else if (light.type == LIGHT_TYPE_POINT)
        return calcPointLight(light, normal, viewDir);
    else if (light.type == LIGHT_TYPE_SPOT)
        return calcSpotLight(light, normal, viewDir);
    else
        return vec3(0.0);
}

void main()
{
    vec3 normal = normalize(FragNorm);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 color = vec3(0.0);
    for (uint i = 0u; i < nLights; i++) {
        color += calcLight(lights[i], normal, viewDir);
    }

    FragColor = vec4(color, material.alpha);
}
