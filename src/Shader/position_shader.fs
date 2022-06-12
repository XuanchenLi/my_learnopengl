#version 330 core
in vec3 ourPosition;
out vec4 FragColor;

void main() {
    FragColor = vec4(ourPosition, 1.0f);
}