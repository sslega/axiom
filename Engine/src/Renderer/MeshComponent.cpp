#include "Renderer/MeshComponent.h"
#include "MeshComponent.h"
#include "Serialization/Archive.h"
#include "Resources/MeshResource.h"
#include "Resources/MaterialResource.h"

namespace axiom
{
    MeshComponent::MeshComponent()
    {
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
