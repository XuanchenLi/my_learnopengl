#version 330 core

in vec3 LightingColor;

uniform vec3 objectColor;

out vec3 FragColor;

void main() {
    FragColor = vec4(LightingColor * objectColor, 1.0);
}