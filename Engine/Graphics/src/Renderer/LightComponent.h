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
        Color color {1.0f, 0.95f, 0.85f};
        float intensity {1.0f};
    };
}