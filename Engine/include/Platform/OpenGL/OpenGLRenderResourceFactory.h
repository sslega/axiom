#pragma once

#include "Renderer/RenderResourceFactory.h"

namespace axiom
{
    class OpenGLRenderResourceFactory : public RenderResourceFactory
    {
        virtual UniquePtr<GraphicsDevice> CreateGraphicsDevice();
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size);
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count);        
        SharedPtr<Shader> CreateShader(const String &vertexSource, const String &fragmentSource);
    };
}