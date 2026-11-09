#include "Resources/MeshResource.h"

namespace axiom
{
    MeshResource::MeshResource()
    {
    }

    MeshResource::MeshResource(Vector<Vertex> vertices, Vector<uint32> indices)
    {
        m_vertices = std::move(vertices);
        m_indices = std::move(indices);
    }

    const Vector<Vertex>& MeshResource::GetVertices() const
    {
        return m_vertices;
    }

    const Vector<uint32>& MeshResource::GetIndices() const
    {
        return m_indices;
    }

}