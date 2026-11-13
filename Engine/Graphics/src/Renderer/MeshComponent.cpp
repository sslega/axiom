#include "Renderer/MeshComponent.h"
#include "MeshComponent.h"
#include "Scene/TransformComponent.h"
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
    
    const Vector<std::type_index>& MeshComponent::GetRequiredComponents() const
    {
        static const Vector<std::type_index> required = { TypeID<TransformComponent>() };
        return required;
    }
}
