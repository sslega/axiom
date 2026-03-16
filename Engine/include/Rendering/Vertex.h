#pragma once

#include "Math/Vector.h"

namespace axiom
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(Vector3 position);
        Vertex(Vector3 position, Vector3 normal);
        Vertex(Vector3 position, Vector3 normal, Vector2 uv);
        Vertex(Vector3 position, Vector3 normal, Vector2 uv, Vector4 color);

        Vector3 m_position;
        Vector3 m_normal;
        Vector2 m_uv;
        Vector4 m_color;
    };
}