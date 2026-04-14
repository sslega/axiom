#pragma once

#include "Math/Vector.h"

namespace axiom
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(Vec3 position);
        Vertex(Vec3 position, Vec3 normal);
        Vertex(Vec3 position, Vec3 normal, Vec2 uv);
        Vertex(Vec3 position, Vec3 normal, Vec2 uv, Vec4 color);

        Vec3 m_position;
        Vec3 m_normal;
        Vec2 m_uv;
        Vec4 m_color;
    };
}
