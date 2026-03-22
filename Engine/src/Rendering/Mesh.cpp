#include "Rendering/Mesh.h"

namespace axiom
{
    Mesh::Mesh()
    {
    }

    Mesh::Mesh(Vector<Vertex> vertices, Vector<uint32> indices)
    {
        m_vertices = std::move(vertices);
        m_indices = std::move(indices);
    }

    const Vector<Vertex> Mesh::GetVertices() const
    {
        return Vector<Vertex>();
    }

    const Vector<uint32> Mesh::GetIndices() const
    {
        return Vector<uint32>();
    }

}