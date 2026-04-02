#pragma once

#include "Core/Types.h"
#include "Renderer/GraphicsDevice.h"

namespace axiom
{
    class VertexBuffer;
    class IndexBuffer;
    class GraphicsDevice;
    class Shader;

    class RenderResourceFactory
    {
    public:
        virtual ~RenderResourceFactory() = default;
        static UniquePtr<RenderResourceFactory> Create(GraphicsDevice::API api);

        virtual UniquePtr<GraphicsDevice> CreateGraphicsDevice() = 0;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) = 0;
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) = 0;        
        virtual SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource) = 0;
    };
}
