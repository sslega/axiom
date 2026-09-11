#pragma once

#include "Core/Types.h"
#include "Math/Vector.h"
#include "Scene/Component.h"

namespace axiom
{
    class Archive;

    class DirectionalLightComponent : public Component
    {
    public:
        Color color {1.0, 0.95, 0.85};
        float intensity {1.0f};
    };
}