#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_height1;
    float shininess;
};

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, TexCoords);
}