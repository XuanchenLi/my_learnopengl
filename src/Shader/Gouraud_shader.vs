#version 330 core
layout (position = 0) in vec3 aPos;
layout (position = 1) in vec3 aNormal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 LightingColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vec3 position = vec3(model * vec4(aPos, 1.0));
    vec3 normal = mat3(transpose(inverse(model))) * aNormal;

    float ambientStrenghth = 0.1;
    ambient = ambientStrenghth * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - position);
    float diff = max(0, dot(lightDir, norm));
    diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDir = vec3(viewPos - position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(0.0, dot(viewDir, reflectDir)), 32);
    vec3 specular = spec * specularStrength * lightColor;

    LightingColor = ambient + specular + diffuse;

}