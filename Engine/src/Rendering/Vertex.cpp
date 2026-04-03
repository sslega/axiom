#include "Rendering/Vertex.h"

namespace axiom
{
    Vertex::Vertex()
    {
    }
    
    Vertex::Vertex(Vec3 position)
    {
        m_position = position;
    }

    Vertex::Vertex(Vec3 position, Vec3 normal)
    {
        m_position = position;
        m_normal = normal;
    }

    Vertex::Vertex(Vec3 position, Vec3 normal, Vec2 uv)
    {
        m_position = position;
        m_normal = normal;
        m_uv = uv;
    }

    Vertex::Vertex(Vec3 position, Vec3 normal, Vec2 uv, Vec4 color)
    {
        m_position = position;
        m_normal = normal;
        m_uv = uv;
        m_color = color;
    }
}

