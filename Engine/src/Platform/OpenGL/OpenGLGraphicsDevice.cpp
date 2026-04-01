#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

namespace axiom
{
    OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
    {
        for (auto& [vb, vao] : m_vaoCache)
            glDeleteVertexArrays(1, &vao);
    }

    void OpenGLGraphicsDevice::SetClearColor(const Vector4 &color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void OpenGLGraphicsDevice::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    uint32 OpenGLGraphicsDevice::GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer)
    {
        VertexBuffer* key = vertexBuffer.get();
        auto it = m_vaoCache.find(key);
        if (it != m_vaoCache.end())
            return it->second;

        uint32 vao;
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
        vertexBuffer->Bind();

        const BufferLayout& layout = vertexBuffer->GetLayout();
        for (uint8 index = 0; index < layout.GetSize(); ++index)
        {
            const auto& element = layout.GetElement(index);
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.offset
            );
        }

        m_vaoCache[key] = vao;
        return vao;
    }

    void OpenGLGraphicsDevice::DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer)
    {
        uint32 vao = GetOrCreateVAO(vertexBuffer);
        glBindVertexArray(vao);
        indexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
