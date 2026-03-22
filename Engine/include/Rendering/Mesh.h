#pragma once

#include "Core/Types.h"
#include "Rendering/Vertex.h"

namespace axiom
{

    class Mesh
    {
    public:
        Mesh();
        Mesh(Vector<Vertex> vertices, Vector<uint32> indices);
        ~Mesh() = default;

        const Vector<Vertex> GetVertices() const;
        const Vector<uint32> GetIndices() const;

    protected:
        Vector<Vertex> m_vertices;
        Vector<uint32> m_indices;
    };
}