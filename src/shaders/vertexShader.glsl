#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec3 aColor;

out VS_OUT {
    vec3 Normal;
    vec3 Color;
    vec3 WorldPos;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 world = model * vec4(aPos, 1.0);
    vs_out.WorldPos = world.xyz;
    vs_out.Normal   = mat3(transpose(inverse(model))) * aNormal;
    vs_out.Color    = aColor;
    gl_Position     = projection * view * world;
}
