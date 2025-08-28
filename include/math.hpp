#pragma once

#include <cmath>
#include <array>

const float PI = 3.14159265359f;

struct Vec3 {
    float x,y,z;
    Vec3():x(0),y(0),z(0){}
    Vec3(float xx,float yy,float zz):x(xx),y(yy),z(zz){}
    Vec3 operator+(const Vec3& o)const{return Vec3(x+o.x,y+o.y,z+o.z);}
    Vec3 operator-(const Vec3& o)const{return Vec3(x-o.x,y-o.y,z-o.z);}
    Vec3 operator*(float s)const{return Vec3(x*s,y*s,z*s);}
};

struct Vec2 { float x,y; };

struct Mat4 {
    std::array<float,16> m;
    Mat4(){ 
        for (int i = 0; i < 16; ++i)
            m[i] = (i % 5 == 0) ? 1.0f:0.0f;
    }
    Mat4(float val) {
        for (int i = 0; i < 16; ++i)
                m[i] = (i % 5 == 0) ? val : 0.0f;
    }
    static Mat4 identity(){return Mat4();}
    
    const float* value_ptr() const {return m.data();}
};

Vec3 cross(const Vec3&a,const Vec3&b);
float dot(const Vec3&a,const Vec3&b);

Mat4 rotate(const Mat4& m, float angle, const Vec3& axis);
Mat4 multiply(const Mat4& m1, const Mat4& m2);

Vec3 normalize(const Vec3&v);

float radians(float degrees);

Mat4 translate(const Vec3&v);
Mat4 scale(const Vec3&v);
Mat4 rotateY(float angle);
Mat4 perspective(float fovy,float aspect,float near,float far);
Mat4 lookAt(const Vec3&eye,const Vec3&center,const Vec3&up);