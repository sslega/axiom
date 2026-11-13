#include "LightComponent.h"
#include "Serialization/Archive.h"
#include "Scene/TransformComponent.h"

namespace axiom
{
    const Vector<std::type_index>& DirectionalLightComponent::GetRequiredComponents() const
    {
        static const Vector<std::type_index> required = { TypeID<TransformComponent>() };
        return required;
    }
}
