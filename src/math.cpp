#include "math.hpp"

vec3 cross(const vec3& a, const vec3& b) {
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float dot(const vec3& a, const vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

vec3 normalize(const vec3& v) {
    float len = std::sqrt(dot(v,v));
    if (len > 1e-6f)
        return vec3(v.x/len, v.y/len, v.z/len);
    return vec3(0,0,0);
}

float radians(float degrees) {
    return degrees * (PI / 180.0f);
}

mat4 multiply(const mat4& m1, const mat4& m2) {
    mat4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i * 4 + j] = m1.m[i * 4 + 0] * m2.m[0 * 4 + j] +
                                 m1.m[i * 4 + 1] * m2.m[1 * 4 + j] +
                                 m1.m[i * 4 + 2] * m2.m[2 * 4 + j] +
                                 m1.m[i * 4 + 3] * m2.m[3 * 4 + j];
        }
    }
    return result;
}

mat4 rotate(const mat4& m, float angle, const vec3& axis) {
    vec3 a = normalize(axis);
    float c = std::cos(angle);
    float s = std::sin(angle);
    float omc = 1.0f - c;

    mat4 rotation_matrix;
    rotation_matrix.m = {
        a.x*a.x*omc + c,      a.x*a.y*omc - a.z*s,  a.x*a.z*omc + a.y*s,  0,
        a.y*a.x*omc + a.z*s,  a.y*a.y*omc + c,      a.y*a.z*omc - a.x*s,  0,
        a.z*a.x*omc - a.y*s,  a.z*a.y*omc + a.x*s,  a.z*a.z*omc + c,      0,
        0,                    0,                    0,                    1
    };
    
    return multiply(m, rotation_matrix);
}

mat4 translate(const vec3& v) {
    mat4 m;
    m.m = {
        1,0,0,v.x,
        0,1,0,v.y,
        0,0,1,v.z,
        0,0,0,1
    };
    return m;
}

mat4 scale(const vec3& v) {
    mat4 m;
    m.m = {
        v.x,0,  0,  0,
        0,  v.y,0,  0,
        0,  0,  v.z,0,
        0,  0,  0,  1
    };
    return m;
}

mat4 rotateY(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat4 m;
    m.m = {
        c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1
    };
    return m;
}

mat4 perspective(float fovy, float aspect, float nearP, float farP) {
    float f = 1.0f / std::tan(fovy * 0.5f);
    mat4 m;
    m.m = {
        f/aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (farP+nearP)/(nearP-farP), (2*farP*nearP)/(nearP-farP),
        0, 0, -1, 0
    };
    return m;
}

mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);

    mat4 m;
    m.m = {
        s.x, u.x, -f.x, 0,
        s.y, u.y, -f.y, 0,
        s.z, u.z, -f.z, 0,
        -dot(s, eye), -dot(u, eye), dot(f, eye), 1
    };
    return m;
}