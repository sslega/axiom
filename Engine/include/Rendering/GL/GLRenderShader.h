#pragma once

#include "Core/Types.h"
#include "Rendering/RenderShader.h"

namespace axiom
{
    class ShaderResource;
    
    class GLRenderShader : public RenderShader
    {
    public:
        GLRenderShader(const ShaderResource& shaderResource);
        GLRenderShader(StringView vertexShader, StringView fragmentShader);
    
        virtual void Bind() const override;
        virtual void Unbind() const override;

    };
}