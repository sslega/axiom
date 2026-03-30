#pragma once
#include "Renderer/RHI.h"
namespace axiom
{
    class OpenGLRHI : public RHI
    {
    public:
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) override;
        virtual SharedPtr<IndexBuffer> CreateIndexBuffer(uint32* indices, uint32 count) override;
    };
}