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
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) const = 0;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(const MeshResource& mesh) const = 0;
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(const MeshResource& mesh) const = 0;
        virtual SharedPtr<VertexBuffer> CreateDynamicVertexBuffer(uint32 byteSize) const = 0;
        
        virtual SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource) const = 0;
        virtual SharedPtr<Shader> CreateShader(const ShaderResource& shaderResource) const = 0;
        
        virtual SharedPtr<Texture2D> CreateTexture2D(const String& path) const = 0;
        virtual SharedPtr<Texture2D> CreateTexture2D(const Texture2DResource& resource) const = 0;

        // Draw commands
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) = 0;
        virtual void DrawIndexedInstanced(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer, const SharedPtr<VertexBuffer>& instanceBuffer, uint32 instanceCount) = 0;

        virtual void Present() = 0;

        inline uint32 GetDrawCallCount()     const { return m_drawCallCount; }
        inline uint32 GetInstanceCallCount() const { return m_instanceCallCount; }
        inline uint32 GetInstanceCount()     const { return m_instanceCount; }

        inline API GetAPI() { return m_API; };

    protected:
        uint32 m_drawCallCount    = 0;
        uint32 m_instanceCallCount = 0;
        uint32 m_instanceCount     = 0;
        void ResetDrawCallCount()
        {
            m_drawCallCount     = 0;
            m_instanceCallCount = 0;
            m_instanceCount     = 0;
        }
    
    private:
        API m_API;
    };
}
