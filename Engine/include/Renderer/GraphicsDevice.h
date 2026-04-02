#pragma once

#include "Math/Vector.h"
#include "Core/Types.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;

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
        static UniquePtr<GraphicsDevice> Create(GraphicsDevice::API api);

        virtual void SetClearColor(const Vector4& color) = 0;
        virtual void Clear() = 0;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) = 0;

        inline API GetAPI() { return m_API; };
    
    private:
        API m_API;
    };
}
