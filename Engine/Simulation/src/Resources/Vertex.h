#pragma once

#include "Math/Vector.h"

namespace axiom
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(Vec3 position);
        Vertex(Vec3 position, Vec2 uv);
        Vertex(Vec3 position, Vec2 uv, Vec3 normal);
        Vertex(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color);

        Vec3 m_position = {0, 0, 0};
        Vec2 m_uv = {0, 0};
        Vec3 m_normal = {0, 0, 0};
        Vec4 m_color = {0, 0, 0, 0};
    };
}
