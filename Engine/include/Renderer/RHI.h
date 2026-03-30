#pragma once

#include "Core/Types.h"

namespace axiom
{
    class VertexBuffer;
    class IndexBuffer;

    class RHI
    {
    public:
        virtual ~RHI() = default;
        virtual SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size) = 0;
        virtual SharedPtr<IndexBuffer> CreateIndexBuffer(uint32* indices, uint32 count) = 0;
    };
}