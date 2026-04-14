#include "Renderer/Vertex.h"

namespace axiom
{
    Vertex::Vertex()
    {
    }

    Vertex::Vertex(Vec3 position)
    {
        m_position = position;
    }

    Vertex::Vertex(Vec3 position, Vec4 color)
    {
        m_position = position;
        m_color    = color;
    }

    Vertex::Vertex(Vec3 position, Vec4 color, Vec2 uv)
    {
        m_position = position;
        m_color    = color;
        m_uv       = uv;
    }
}
