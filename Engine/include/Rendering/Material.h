#pragma once
#include "Core/Types.h"
#include "Rendering/RenderShader.h"

namespace axiom
{
    struct Material
    {
        SharedPtr<RenderShader> shader;
    };
} // namespace axiom
