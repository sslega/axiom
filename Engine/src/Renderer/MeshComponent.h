#pragma once
#include "Scene/Component.h"
#include "Core/Types.h"

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

        SharedPtr<MeshResource> mesh;
        SharedPtr<MaterialResource> material;

    protected:

        bool m_isVisible = true;
    };
}
