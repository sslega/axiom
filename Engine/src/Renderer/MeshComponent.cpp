#include "Renderer/MeshComponent.h"
#include "MeshComponent.h"
#include "Serialization/Archive.h"
#include "Resources/MeshResource.h"
#include "Renderer/Material.h"

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
    
    void MeshComponent::Deserialize(Archive &ar)
    {
        ar.Read("material", m_materialID);
        ar.Read("visible", m_isVisible);
        ar.Read("mesh", m_mesh);
    }

    void MeshComponent::Serialize(Archive &ar)
    {
        ar.Write("material", m_materialID);
        ar.Write("visible", m_isVisible);
        ar.Write("mesh", m_mesh);
    }
}