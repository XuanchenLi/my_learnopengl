#version 330 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float ratio;

void main() {
    FragColor = mix(texture(ourTexture1, texCoord),  texture(ourTexture2, texCoord), ratio);
}