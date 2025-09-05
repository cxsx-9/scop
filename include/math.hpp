#pragma once

#include <cmath>
#include <array>
#include <float.h>

namespace mymath {
    static constexpr float PI = 3.14159265359f;
    static constexpr float EPS = 1e-6f;

    struct vec2 {
        float x,y;
        vec2():x(0),y(0){}
        vec2(float aa):x(aa),y(aa){}
        vec2(float xx,float yy):x(xx),y(yy){}
        bool operator==(const vec2& o) const { return x == o.x && y == o.y; }
    };

    struct vec3 {
        float x,y,z;
        vec3():x(0),y(0),z(0){}
        vec3(float aa):x(aa),y(aa),z(aa){}
        vec3(float xx,float yy,float zz):x(xx),y(yy),z(zz){}
        vec3 operator+(const vec3& o) const { return vec3(x+o.x, y+o.y, z+o.z); }
        vec3 operator-(const vec3& o) const { return vec3(x-o.x, y-o.y, z-o.z); }
        vec3 operator*(float s) const { return vec3(x*s, y*s, z*s); }
        vec3 operator/(float s) const { return vec3(x/s, y/s, z/s); }
        bool operator==(const vec3& o) const { return x == o.x && y == o.y && z == o.z; }
        vec3& operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        vec3& operator-=(const vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    };
    
    struct mat4 {
        // column-major: m[c*4 + r]
        std::array<float, 16> m;
        mat4() { m.fill(0.0f); }
        mat4(float diag) { m.fill(0.0f); m[0] = m[5] = m[10] = m[15] = diag; }
        static mat4 identity() {
            mat4 I; I.m[0] = I.m[5] = I.m[10] = I.m[15] = 1.0f; return I;
        }
        float* data() { return m.data(); }
        const float* data() const { return m.data(); }

        mat4 operator*(const mat4& o) const {
            mat4 R;
            for (int r = 0; r < 4; ++r) {
                for (int c = 0; c < 4; ++c) {
                    R.m[c*4 + r] = 
                        m[0*4 + r] * o.m[c*4 + 0] +
                        m[1*4 + r] * o.m[c*4 + 1] +
                        m[2*4 + r] * o.m[c*4 + 2] +
                        m[3*4 + r] * o.m[c*4 + 3];
                }
            }
            return R;
        }

        mat4 operator*=(const mat4& o) {
            *this = *this * o;
            return *this;
        }

    };

    float dot(const vec3& a, const vec3& b);
    vec3 cross(const vec3& a, const vec3& b);

    mymath::vec3 min(const mymath::vec3& a, const mymath::vec3& b);
    mymath::vec3 max(const mymath::vec3& a, const mymath::vec3& b);
    float length(const vec3& v);
    vec3 normalize(const vec3& v);

    // Mat4
    // translate / rotate / scale
    mat4 translate(const mat4& m, const vec3& t);
    mat4 rotateAxisAngle(const vec3& axis, float angle);
    mat4 rotate(const mat4& m, float angle, const vec3& axis);
    mat4 scale(const vec3& s);

    // lookAt (vec3 eye, vec3 center, vec3 up)
    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up);
    // perspective (float fovy, float aspect, float zNear, float zFar)
    mat4 perspective(float fovy, float aspect, float zNear, float zFar);
    float radians(float degrees);
}
