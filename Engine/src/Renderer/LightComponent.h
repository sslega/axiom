#pragma once

#include "Core/Types.h"
#include "Core/Reflection.h"
#include "Math/Vector.h"
#include "Scene/Component.h"

namespace axiom
{
    class Archive;

    class DirectionalLightComponent : public Component
    {
    public:
        Vec3 color {1.0, 0.95, 0.85};
        float intensity {1.0f};

        virtual void Serialize(Archive& ar) override;
        virtual void Deserialize(Archive& ar) override;
    };

    AX_REFLECT(DirectionalLightComponent)
        AX_FIELD(color)
        AX_FIELD(intensity)
    AX_REFLECT_END()
}