#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;  //计算世界坐标系中的法线 变换矩阵为模型矩阵的逆的转置
    FragPos = vec3(model * vec4(aPos, 1.0));  //计算世界坐标系中的片段位置
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}