#pragma once

namespace axiom
{
    struct Vector2
    {
        float x, y;
        Vector2() : x(0), y(0) {}
        Vector2(float _x, float _y) : x(_x), y(_y) {}
    };

     struct Vector3
    {
        float x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
    };

    struct Vector4
    {
        float x, y, z, w;
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
    };
}