#version 330 core
in VS_OUT {
    vec3 Normal;
    vec3 Color;
    vec3 WorldPos;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightDir = normalize(vec3(-0.4, -1.0, -0.3));

void main() {
    vec3 N = normalize(fs_in.Normal);
    vec3 L = normalize(-lightDir);
    float diff = max(dot(N, L), 0.0);

    vec3 base = fs_in.Color;
    vec3 color = base * (0.2 + 0.8 * diff);
    FragColor = vec4(color, 1.0);
}
