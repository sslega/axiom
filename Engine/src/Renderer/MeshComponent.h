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
        const SharedPtr<MeshResource> GetMesh() const;
        void SetMaterial(const SharedPtr<Material>& material);
        const SharedPtr<Material> GetMaterial() const;
        bool IsVisible() const;
        void SetVisible(bool visible);

        virtual void Deserialize(Archive& ar) override;
        virtual void Serialize(Archive& ar) override;

    protected:
        friend class SceneLoader;
        SharedPtr<MeshResource> m_mesh;
        SharedPtr<Material> m_material;
        //TODO: remove this m_materialID from member, it should be part of resource
        String m_materialID;
        bool m_isVisible = true;
    };
}
