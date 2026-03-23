#pragma once

#include "Core/Types.h"
#include "Rendering/RenderShader.h"

namespace axiom
{
    class GLShader : public RenderShader
    {
    public:
        GLShader(StringView vertexShader, StringView fragmentShader);

        virtual void Bind() const override;
        virtual void Unbind() const override;

    };
}