#pragma once
#include <cmath>

namespace axiom
{
    //TODO: add rest of the operators
    struct Vec2
    {
        float x, y;
        Vec2() : x(0), y(0) {}
        Vec2(float xy) : x(xy), y(xy) {}
        Vec2(float _x, float _y) : x(_x), y(_y) {}
        Vec2  operator-()            const { return Vec2(-x, -y); }
        Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
        Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
        Vec2& operator*=(float s)       { x *= s;   y *= s;   return *this; }
        bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
        bool operator!=(const Vec2& o) const { return !(*this == o); }
    };

    struct Vec3
    {
        float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
        Vec3(float xyz) : x(xyz), y(xyz), z(xyz){}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vec3  operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
        Vec3  operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
        Vec3  operator*(float s)       const { return Vec3(x * s,   y * s,   z * s);   }
        Vec3  operator/(float s)       const { return Vec3(x / s,   y / s,   z / s);   }
        Vec3  operator-()              const { return Vec3(-x, -y, -z); }
        Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
        Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
        Vec3& operator*=(float s)       { x *= s;   y *= s;   z *= s;   return *this; }
        Vec3& operator/=(float s)       { x /= s;   y /= s;   z /= s;   return *this; }
        bool operator==(const Vec3& o) const { return x == o.x && y == o.y && z == o.z; }
        bool operator!=(const Vec3& o) const { return !(*this == o); }
        Vec2 xy() const { return Vec2(x, y); }
    };

    struct Vec4
    {
        float x, y, z, w;
        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
        Vec4  operator-()              const { return Vec4(-x, -y, -z, -w); }
        Vec4& operator+=(const Vec4& o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
        Vec4& operator-=(const Vec4& o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
        Vec4& operator*=(float s)       { x *= s;   y *= s;   z *= s;   w *= s;   return *this; }
        Vec4& operator/=(float s)       { x /= s;   y /= s;   z /= s;   w /= s;   return *this; }
        bool operator==(const Vec4& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
        bool operator!=(const Vec4& o) const { return !(*this == o); }
        Vec3 xyz() const { return Vec3(x, y, z); }
    };

    inline float Dot(Vec3 a, Vec3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline Vec3 Cross(Vec3 a, Vec3 b)
    {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    inline float Length(Vec3 v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    inline Vec3 Normalize(Vec3 v)
    {
        float len = Length(v);
        return Vec3(v.x / len, v.y / len, v.z / len);
    }
}
