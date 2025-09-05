#include "math.hpp"

namespace mymath {

    // Vec3
    float dot(const vec3& a, const vec3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}

    vec3 cross(const vec3& a, const vec3& b) {
        return vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    mymath::vec3 min(const mymath::vec3& a, const mymath::vec3& b) {
        return mymath::vec3(
            std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z)
        );
    }

    mymath::vec3 max(const mymath::vec3& a, const mymath::vec3& b) {
        return mymath::vec3(
            std::max(a.x, b.x),
            std::max(a.y, b.y),
            std::max(a.z, b.z)
        );
    }

    mymath::vec3 abs(const mymath::vec3& v) {
        return mymath::vec3(
            std::fabs(v.x),
            std::fabs(v.y),
            std::fabs(v.z)
        );
    }

    float length(const vec3& v) { return std::sqrt(dot(v,v)); }

    vec3 normalize(const vec3& v) {
        float L = length(v);
        if (L < EPS) return vec3(0,0,0);
        return v / L;
    }

    // Mat4
    // translate / rotate / scale
    mat4 translate(const mat4& m, const vec3& t) {
        mat4 M = mat4::identity();
        M.m[12] = t.x; M.m[13] = t.y; M.m[14] = t.z;
        return M * m;
    }

    mat4 rotateAxisAngle(const vec3& axis, float angle) {
        vec3 a = normalize(axis);
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1.0f - c;
        mat4 R; R.m.fill(0.0f);

        R.m[0] = t*a.x*a.x + c;
        R.m[1] = t*a.x*a.y + s*a.z;
        R.m[2] = t*a.x*a.z - s*a.y;
        R.m[3] = 0.0f;

        R.m[4] = t*a.x*a.y - s*a.z;
        R.m[5] = t*a.y*a.y + c;
        R.m[6] = t*a.y*a.z + s*a.x;
        R.m[7] = 0.0f;

        R.m[8] = t*a.x*a.z + s*a.y;
        R.m[9] = t*a.y*a.z - s*a.x;
        R.m[10] = t*a.z*a.z + c;
        R.m[11] = 0.0f;

        R.m[12] = R.m[13] = R.m[14] = 0.0f;
        R.m[15] = 1.0f;
        return R;
    }

    mat4 rotate(const mat4& m, float angle, const vec3& axis) {
        return m * rotateAxisAngle(axis, angle);
    }

    mat4 scale(const vec3& s) {
        mat4 M; M.m.fill(0.0f);
        M.m[0] = s.x; M.m[5] = s.y; M.m[10] = s.z; M.m[15] = 1.0f;
        return M;
    }

    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        vec3 f = normalize(center - eye);
        vec3 s = normalize(cross(f, up));
        vec3 u = cross(s, f);

        mat4 Result = mat4::identity();
        // columns
        Result.m[0] = s.x; Result.m[1] = u.x; Result.m[2] = -f.x; Result.m[3] = 0.0f;
        Result.m[4] = s.y; Result.m[5] = u.y; Result.m[6] = -f.y; Result.m[7] = 0.0f;
        Result.m[8] = s.z; Result.m[9] = u.z; Result.m[10] = -f.z; Result.m[11] = 0.0f;
        Result.m[12] = -dot(s, eye);
        Result.m[13] = -dot(u, eye);
        Result.m[14] = dot(f, eye);
        Result.m[15] = 1.0f;
        return Result;
    }

    mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
        float tanHalfFovy = std::tan(fovy / 2.0f);
        mat4 Result; Result.m.fill(0.0f);
        Result.m[0]  = 1.0f / (aspect * tanHalfFovy);
        Result.m[5]  = 1.0f / (tanHalfFovy);
        Result.m[10] = - (zFar + zNear) / (zFar - zNear);
        Result.m[11] = -1.0f;
        Result.m[14] = - (2.0f * zFar * zNear) / (zFar - zNear);
        Result.m[15] = 0.0f;
        return Result;
    }

    float radians(float degrees) {
        return degrees * (PI / 180.0f);
    }
}