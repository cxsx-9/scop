#version 330 core
in VS_OUT {
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform bool useTexture;
uniform sampler2D diffuseMap;
uniform vec3 baseColor;

const vec3 lightDir = normalize(vec3(-0.4, -1.0, -0.3));
const float ambientK = 0.2;
const float diffuseK = 0.8;

void main() {
    vec3 N = normalize(fs_in.Normal);
    vec3 L = normalize(-lightDir);
    float diff = max(dot(N, L), 0.0);

    vec3 base = useTexture
        ? texture(diffuseMap, fs_in.TexCoord).rgb
        : baseColor;

    vec3 color = base * (ambientK + diffuseK * diff);
    FragColor = vec4(color, 1.0);
}
