#include "Rendering/Vertex.h"

namespace axiom
{
    Vertex::Vertex()
    {
    }
    
    Vertex::Vertex(Vector3 position)
    {
        m_position = position;
    }
    
    Vertex::Vertex(Vector3 position, Vector3 normal)
    {
        m_position = position;
        m_normal = normal;
    }

    Vertex::Vertex(Vector3 position, Vector3 normal, Vector2 uv)
    {
        m_position = position;
        m_normal = normal;
        m_uv = uv;
    }

    Vertex::Vertex(Vector3 position, Vector3 normal, Vector2 uv, Vector4 color)
    {
        m_position = position;
        m_normal = normal;
        m_uv = uv;
        m_color = color;
    }
}

