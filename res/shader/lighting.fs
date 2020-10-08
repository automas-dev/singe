#version 330 core

uniform sampler2D tex;

in vec3 ourPos;
in vec2 ourTex;
in vec3 ourNorm;

out vec4 FragColor;

const float ambientStrength = 1.0;

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

    vec3 norm = normalize(ourNorm);
    vec3 lightDir = normalize(lightPos - ourPos);

    float diff = max(dot(norm, lightDir), 0.0);
    color += diff * diffuse;

    vec3 viewDir = normalize(viewPos - ourPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specExp);
    color += spec * specular;

    FragColor = vec4(color, alpha);
    // FragColor = vec4(ourTex.x, 0.0, ourTex.y, alpha);
    // FragColor = vec4(color, alpha) * texture(tex, ourTex);
    // FragColor = (vec4(diffuse, 1.0) + vec4(specular, 1.0));
    // FragColor = texture(tex, ourTex);
}
