#version 330 core

in vec3 ourPos;
in vec2 ourTex;
in vec3 ourNorm;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(0.8), 1.0);
    // FragColor = (vec4(diffuse, 1.0) + vec4(specular, 1.0));
}
