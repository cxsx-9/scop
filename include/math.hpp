#pragma once

#include <cmath>
#include <array>

const float PI = 3.14159265359f;


struct vec2 { 
    float x,y; 
    vec2():x(0),y(0){}
    vec2(float aa):x(aa),y(aa){}
    vec2(float xx,float yy):x(xx),y(yy){}
};

struct vec3 {
    float x,y,z;
    vec3():x(0),y(0),z(0){}
    vec3(float aa):x(aa),y(aa),z(aa){}
    vec3(float xx,float yy,float zz):x(xx),y(yy),z(zz){}
    vec3 operator+(const vec3& o)const{return vec3(x+o.x,y+o.y,z+o.z);}
    vec3 operator-(const vec3& o)const{return vec3(x-o.x,y-o.y,z-o.z);}
    vec3 operator*(float s)const{return vec3(x*s,y*s,z*s);}
};

vec3 normalize(const vec3&v);
vec3 cross(const vec3&a,const vec3&b);
float radians(float degrees);
/* 
vec3 :
    radians
    normalize
    cos
*/



struct mat4 {
    std::array<float,16> m;
    mat4(){ 
        for (int i = 0; i < 16; ++i)
            m[i] = (i % 5 == 0) ? 1.0f:0.0f;
    }
    mat4(float val) {
        for (int i = 0; i < 16; ++i)
                m[i] = (i % 5 == 0) ? val : 0.0f;
    }
    static mat4 identity(){return mat4();}
    
    const float* value_ptr() const {return m.data();}
};

float dot(const vec3&a,const vec3&b);

mat4 rotate(const mat4& m, float angle, const vec3& axis);
mat4 multiply(const mat4& m1, const mat4& m2);



mat4 translate(const vec3&v);
mat4 scale(const vec3&v);
mat4 rotateY(float angle);
mat4 perspective(float fovy,float aspect,float near,float far);
mat4 lookAt(const vec3&eye,const vec3&center,const vec3&up);