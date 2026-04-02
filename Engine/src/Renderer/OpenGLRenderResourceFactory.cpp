#include "Platform/OpenGL/OpenGLRenderResourceFactory.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLGraphicsDevice.h"

namespace axiom
{
    UniquePtr<GraphicsDevice> OpenGLRenderResourceFactory::CreateGraphicsDevice()
    {
        return MakeUnique<OpenGLGraphicsDevice>();
    }

    SharedPtr<VertexBuffer> OpenGLRenderResourceFactory::CreateVertexBuffer(float *vertices, uint32 size)
    {
        return MakeShared<OpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<IndexBuffer> OpenGLRenderResourceFactory::CreateIndexBuffer(uint32 *indices, uint32 count)
    {
        return MakeShared<OpenGLIndexBuffer>(indices, count);
    }

    SharedPtr<Shader> OpenGLRenderResourceFactory::CreateShader(const String &vertexSource, const String &fragmentSource)
    {
        return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
    }
}