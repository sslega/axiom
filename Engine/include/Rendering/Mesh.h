#pragma once

#include "Core/Types.h"
#include "Vertex.h"

namespace axiom
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh() = default;

    protected:
        Vector<Vertex> m_vertices;
        Vector<uint16> m_indices;
    };
}