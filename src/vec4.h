#ifndef VEC4_H
#define VEC4_H
#include <algorithm>
#include "vec3.h"

template <typename T>
class Vec4: public Vec3<T>{
public:
    Vec4() = default;

    Vec4(T x_, T y_, T z_, T w_ = 1.0f): Vec3<T>(x_, y_, z_), w(w_){}

    Vec4(const Vec3<T>& v, float w_ = 1.0f): Vec3<T>(v.x, v.y, v.z), w(w_){}

    template <typename T2>
    explicit operator Vec4<T2>() const{
        return Vec4(T2(this->x), T2(this->y), T2(this->z), T2(w));
    }

    Vec4 operator -() const{
        return Vec4(-this->x, -this->y, -this->z, -w);
    }

    Vec4& operator =(const Vec4& v){
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
        return *this;
    }

    Vec4 operator +(const Vec4& v){
        return Vec4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
    }

    Vec4& operator +=(const Vec4& v){
        *this = *this + v;
        return *this;
    }

    Vec4 operator -(const Vec4& v){
        return Vec4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
    }

    Vec4& operator -=(const Vec4& v){
        *this = *this - v;
        return *this;
    }

    Vec4 operator *(const T& val){
        return Vec4(this->x * val, this->y * val, this->z * val, this->w * val);
    }

    Vec4& operator *=(const T& val){
        *this = *this * val;
        return *this;
    }

    Vec4 operator /(const T& val){
        return Vec4(this->x / val, this->y / val, this->z / val, this->w / val);
    }

    Vec4& operator /=(const T& val){
        *this = *this / val;
        return *this;
    }

    Vec4& operator ==(const Vec4& v) const{
        return this->x == v.x and this->y == v.y and this->z == v.z and this->w == v.w;
    }

    Vec4& operator !=(const Vec4& v) const{
        return !(*this == v);
    }

public:
    T w;
};

using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
using Vec4i = Vec4<int>;

#endif // VEC4_H
