#pragma once
#include "Scene/Component.h"
#include "Core/Types.h"
#include "Resources/ResourceRef.h"

namespace axiom
{
    class MeshResource;
    class MaterialResource;

    class MeshComponent: public Component
    {
    public:
        MeshComponent();

        bool IsVisible() const;
        void SetVisible(bool visible);

        ResourceRef<MeshResource> mesh;
        ResourceRef<MaterialResource> material;

    protected:

        bool m_isVisible = true;
    };
}
