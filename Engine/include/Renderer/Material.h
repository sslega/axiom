#pragma once
#include "Core/Types.h"
#include "Resources/ShaderResource.h"

namespace axiom
{
    struct Material
    {
        SharedPtr<ShaderResource> shader;
    };
} // namespace axiom
