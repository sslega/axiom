#pragma once

#include "Renderer/Buffer.h"

namespace axiom
{
    inline BufferLayout GetVertexLayout()
    {
        return {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float4, "a_Color"}
        };
    }
}
