#pragma once
#include "Scene/Component.h"
#include "Core/Types.h"

namespace axiom
{
    class MeshResource;
    class Material;
    
    class MeshComponent: public Component
    {
    public:
        MeshComponent();
        void SetMesh(const SharedPtr<MeshResource>& mesh);
        const SharedPtr<MeshResource>& GetMesh() const;
        void SetMaterial(const SharedPtr<Material>& material);
        const SharedPtr<Material>& GetMaterial() const;
        bool IsVisible() const;
        void SetVisible(bool visible);

    protected:
        SharedPtr<MeshResource> m_mesh;
        SharedPtr<Material> m_material;
        bool m_isVisible;
    };
}