#pragma once

#include "Core/Types.h"
#include "Math/Vector.h"
#include "Scene/Component.h"

namespace axiom
{
    class DirectionalLight : public Component
    {
    public:
        Vec3 color;
        float intensity;
    };
}