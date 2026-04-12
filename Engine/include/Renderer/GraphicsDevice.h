#pragma once

#include "Math/Vector.h"
#include "Core/Types.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class ApplicationWindow;

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
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) = 0;
        virtual SharedPtr<IndexBuffer>  CreateIndexBuffer(uint32* indices, uint32 count) = 0;        
        virtual SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource) = 0;

        // Draw commands
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) = 0;
        virtual void Present() = 0;

        // ImGui renderer backend — implemented per graphics API
        // virtual void InitImGuiRenderer()       {}
        // virtual void ShutdownImGuiRenderer()   {}
        // virtual void BeginImGuiRendererFrame() {}
        // virtual void EndImGuiFrame()           {}

        inline API GetAPI() { return m_API; };
    
    private:
        API m_API;
    };
}
