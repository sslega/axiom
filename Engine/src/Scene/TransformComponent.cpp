#include "TransformComponent.h"
#include "Serialization/Archive.h"
#include "Serialization/ClassRegistry.h"

namespace axiom
{
    void TransformComponent::Serialize(Archive &ar)
    {
        ar.Write("position", position);
        ar.Write("scale", scale);
        ar.Write("rotation", rotation);
    }

    void TransformComponent::Deserialize(Archive &ar)
    {
        ar.Read("position", position);
        ar.Read("scale", scale);
        ar.Read("rotation", rotation);
    }
}
