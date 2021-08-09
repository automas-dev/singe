
vec3 lAmbient = vec3(0.3, 0.3, 0.3);
vec3 lDiffuse = vec3(0.8, 0.8, 0.8);
vec3 lSpecular = vec3(0.8, 0.8, 0.8);

vec3 lightAmbient(vec3 albedo) {
    return albedo * lAmbient;
}

vec3 lightDirectional(vec3 pos, vec3 fragAlbedo, float fragSpecular, vec3 lightPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(lightPos - pos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragSpecular);

    // combine results
    vec3 diffuse = lDiffuse * fragAlbedo * diff;
    vec3 specular = lSpecular * fragSpecular * spec;

    return (diffuse + specular);
}