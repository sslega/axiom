#include "Scene/MeshComponent.h"

namespace axiom
{
    MeshComponent::MeshComponent()
    {
    }

    void MeshComponent::SetMesh(const SharedPtr<Mesh>& mesh)
    {
        m_mesh = mesh;
    }

    const SharedPtr<Mesh>& MeshComponent::GetMesh() const
    {
        return m_mesh;
    }
}
