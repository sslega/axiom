#pragma once

#include "Core/Types.h"
#include "Rendering/Vertex.h"

namespace axiom
{

    class MeshResource
    {
    public:
        MeshResource();
        MeshResource(Vector<Vertex> vertices, Vector<uint32> indices);
        ~MeshResource() = default;

        const Vector<Vertex> GetVertices() const;
        const Vector<uint32> GetIndices() const;

    protected:
        Vector<Vertex> m_vertices;
        Vector<uint32> m_indices;
    };
}