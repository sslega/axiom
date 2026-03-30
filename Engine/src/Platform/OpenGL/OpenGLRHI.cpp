#include "Platform/OpenGL/OpenGLRHI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace axiom
{
    SharedPtr<VertexBuffer> OpenGLRHI::CreateVertexBuffer(float* vertices, uint32 size)
    {
        return MakeShared<OpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<IndexBuffer> OpenGLRHI::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        return MakeShared<OpenGLIndexBuffer>(indices, count);
    }
}
