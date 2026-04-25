#pragma once

#include "Renderer/GraphicsDevice.h"
#include "Platform/ApplicationWindow.h"
#include "Core/Types.h"
#include <unordered_map>

class GLFWwindow;

namespace axiom
{
    class ShaderResource;
    class Texture2DResource;

    class OpenGLGraphicsDevice : public GraphicsDevice
    {
    public:
        OpenGLGraphicsDevice(const ApplicationWindow& window);
        ~OpenGLGraphicsDevice();

        void SetClearColor(const Vec4& color) override;
        void Clear() override;
        void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) override;
        void DrawIndexedInstanced(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer, const SharedPtr<VertexBuffer>& instanceBuffer, uint32 instanceCount) override;
        void SwapBuffers() override;

        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(const MeshResource& mesh) const override;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) const override;        
        virtual SharedPtr<VertexBuffer> CreateDynamicVertexBuffer(uint32 byteSize) const override;

        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(const MeshResource& mesh) const override;
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) const override;
        virtual SharedPtr<IndexBuffer>  CreateDynamicIndexBuffer(uint32 maxCount) const override;
        
        SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource) const override;
        SharedPtr<Shader> CreateShader(const ShaderResource& shaderResource) const override;
        
        SharedPtr<Texture2D> CreateTexture2D(const String& path) const override;
        SharedPtr<Texture2D> CreateTexture2D(const Texture2DResource& resource) const override;

        virtual SharedPtr<FrameBuffer> CreateFrameBuffer(const FramebufferSpec& spec) const override;
        virtual void SetDepthTestEnabled(bool enabled) override;
        virtual void BindFrameBufferTexture(FrameBuffer& frameBuffer, uint32 slot) override;

    private:
        PairMap<std::pair<VertexBuffer*, VertexBuffer*>, uint32> m_vaoCache;
        GLFWwindow* m_windowHandle;
        uint32 GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer);
        uint32 GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<VertexBuffer>& instanceBuffer);
    };
}