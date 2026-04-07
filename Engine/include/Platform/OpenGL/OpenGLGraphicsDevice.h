#pragma once

#include "Renderer/GraphicsDevice.h"
#include "Platform/ApplicationWindow.h"
#include "Core/Types.h"
#include <unordered_map>

class GLFWwindow;

namespace axiom
{
    class OpenGLGraphicsDevice : public GraphicsDevice
    {
    public:
        OpenGLGraphicsDevice(const ApplicationWindow& window);
        ~OpenGLGraphicsDevice();

        virtual void SetClearColor(const Vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) override;
        virtual void Present() override; 

        // virtual UniquePtr<GraphicsDevice> CreateGraphicsDevice() override;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) override;
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) override;        
        SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource)  override;

    private:
        UnorderedMap<VertexBuffer*, uint32> m_vaoCache;
        GLFWwindow* m_windowHandle;
        uint32 GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer);
    };
}