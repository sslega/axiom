#include "Renderer/MeshComponent.h"

namespace axiom
{
    MeshComponent::MeshComponent()
    {
    }

    void MeshComponent::SetMesh(const SharedPtr<MeshResource>& mesh)
    {
        m_mesh = mesh;
    }

    const SharedPtr<MeshResource> MeshComponent::GetMesh() const
    {
        return m_mesh;
    }

    void MeshComponent::SetMaterial(const SharedPtr<Material>& material)
    {
        m_material = material;
    }

    const SharedPtr<Material> MeshComponent::GetMaterial() const
    {
        return m_material;
    }

    bool MeshComponent::IsVisible() const
    {
        return m_isVisible;
    }

    void MeshComponent::SetVisible(bool visible)
    {
        m_isVisible = visible;
    }

}
