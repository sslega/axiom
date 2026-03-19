#pragma once

#include "Core/Types.h"
#include "Shader.h"

namespace axiom
{
    class GLShader : public Shader
    {
    public:
        GLShader(StringView vertexShader, StringView fragmentShader);

        virtual void Bind() const override;
        virtual void Unbind() const override;

    };
}