#pragma once
#include "Renderer/GraphicsDevice.h"
#include <unordered_map>

namespace axiom
{
    class OpenGLGraphicsDevice : public GraphicsDevice
    {
    public:
        ~OpenGLGraphicsDevice();

        virtual void SetClearColor(const Vector4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer) override;

    private:
        uint32 GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer);
        std::unordered_map<VertexBuffer*, uint32> m_vaoCache;
    };
}