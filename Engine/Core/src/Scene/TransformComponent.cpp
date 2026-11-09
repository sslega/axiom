#include "TransformComponent.h"
#include "Serialization/Archive.h"
#include "Math/Math.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    void TransformComponent::Serialize(Archive &ar)
    {
        Component::Serialize(ar);
        Vec3 rotationDegrees = ToDegrees(rotation);
        ar.Write("rotation", rotationDegrees);
    }

    void TransformComponent::Deserialize(Archive &ar)
    {
        Component::Deserialize(ar);
        Vec3 rotationDegrees;
        ar.Read("rotation", rotationDegrees);
        rotation = ToRadians(rotationDegrees);
    }
}