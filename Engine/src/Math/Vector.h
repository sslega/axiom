#pragma once
#include <cmath>

namespace axiom
{
    struct Vec2
    {
        float x, y;
        Vec2() : x(0), y(0) {}
        Vec2(float xy) : x(xy), y(xy) {}
        Vec2(float _x, float _y) : x(_x), y(_y) {}
    };

    struct Vec3
    {
        float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
        Vec3(float xyz) : x(xyz), y(xyz), z(xyz){}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
        Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
        Vec3 operator*(float s)       const { return Vec3(x * s,   y * s,   z * s);   }
    };

    struct Vec4
    {
        float x, y, z, w;
        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
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
