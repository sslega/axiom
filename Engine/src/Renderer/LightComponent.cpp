#include "LightComponent.h"
#include "Serialization/Archive.h"

namespace axiom
{
    void DirectionalLightComponent::Serialize(Archive& ar)
    {
        ar.Write("color", color);
        ar.Write("intensity", intensity);
    }

    void DirectionalLightComponent::Deserialize(Archive& ar)
    {
        ar.Read("color", color);
        ar.Read("intensity", intensity);
    }
}
