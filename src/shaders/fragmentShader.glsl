#version 330 core
in VS_OUT {
    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

out vec4 FragColor;

uniform int prevMode;
uniform int renderMode;
uniform float mixFactor;
uniform sampler2D diffuseMap;

vec3 getFaceColor() {
    int faceID = gl_PrimitiveID;
    float r = float((faceID * 97) % 255) / 255.0;
    float g = float((faceID * 57) % 255) / 255.0;
    float b = float((faceID * 23) % 255) / 255.0;
    return vec3(r, g, b);
}

vec3 getModeColor(int mode) {
    if (mode == 1) return vec3(0.7);
    if (mode == 2) return getFaceColor();
    if (mode == 3) return texture(diffuseMap, fs_in.TexCoord).rgb;
    return vec3(1.0);
}

void main() {
    vec3 base = mix(getModeColor(prevMode), getModeColor(renderMode), mixFactor);

    vec3 N = normalize(fs_in.Normal);
    vec3 L = normalize(vec3(-0.4, -1.0, -0.3));
    float diff = max(dot(N, -L), 0.0);

    vec3 finalColor = base * (0.2 + 0.8 * diff);
    FragColor = vec4(finalColor, 1.0);
}
