#pragma once

#include "Math/Vector.h"
#include "Core/Types.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class ShaderResource;
    class Texture2D;
    class Texture2DResource;
    class ApplicationWindow;
    class MeshResource;
    class FrameBuffer;

    struct FramebufferSpec;

    enum class DepthFunction { Less, LessEqual, Equal };

    class GraphicsDevice 
    {
    public:
        enum class API
        {
            None,
            OpenGL,
            Vulkan,
            DX12
        };
        static UniquePtr<GraphicsDevice> Create(GraphicsDevice::API api, const ApplicationWindow& window);

        // Resource creation
        // virtual UniquePtr<GraphicsDevice> CreateGraphicsDevice() = 0;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 byteSize) const = 0;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(const MeshResource& mesh) const = 0;
        virtual SharedPtr<VertexBuffer> CreateDynamicVertexBuffer(uint32 byteSize) const = 0;

        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) const = 0;        
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(const MeshResource& mesh) const = 0;
        virtual SharedPtr<IndexBuffer>  CreateDynamicIndexBuffer(uint32 maxCount) const = 0;
        
        virtual SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource) const = 0;
        virtual SharedPtr<Shader> CreateShader(const ShaderResource& shaderResource) const = 0;
        
        virtual SharedPtr<Texture2D> CreateTexture2D(const String& path) const = 0;
        virtual SharedPtr<Texture2D> CreateTexture2D(const Texture2DResource& resource) const = 0;

        virtual SharedPtr<FrameBuffer> CreateFrameBuffer(const FramebufferSpec& spec) const = 0;

        // Draw commands
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) = 0;
        virtual void DrawIndexedInstanced(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer, const SharedPtr<VertexBuffer>& instanceBuffer, uint32 instanceCount) = 0;
        virtual void BindFrameBufferTexture(FrameBuffer& frameBuffer, uint32 slot) = 0;

        virtual void SetDepthTestEnabled(bool enabled) = 0;
        virtual void SetColorWriteEnabled(bool enabled) = 0;
        virtual void SetDepthWriteEnabled(bool enabled) = 0;

        virtual void SetDepthFunction(DepthFunction func) = 0;

        virtual void SwapBuffers() = 0;

        inline API GetAPI() { return m_API; };
    
    private:
        API m_API;
    };
}
