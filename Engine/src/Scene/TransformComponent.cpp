#include "TransformComponent.h"
#include "Serialization/Archive.h"
#include "Serialization/ClassRegistry.h"
#include "Math/Math.h"

namespace axiom
{
    void TransformComponent::Serialize(Archive &ar)
    {
        ar.Write("position", position);
        ar.Write("scale", scale);
        Vec3 rotationDegrees = ToDegrees(rotation);
        ar.Write("rotation", rotationDegrees);
    }

    void TransformComponent::Deserialize(Archive &ar)
    {
        ar.Read("position", position);
        ar.Read("scale", scale);
        Vec3 rotationDegrees;
        ar.Read("rotation", rotationDegrees);
        rotation = ToRadians(rotationDegrees);
    }
}
