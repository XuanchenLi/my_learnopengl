#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNoraml;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    mat3 matrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(projection * vec4(matrix * aNoraml, 1.0)));
}