#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/glad.h>

namespace axiom
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32 size)
    {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    const BufferLayout& OpenGLVertexBuffer::GetLayout() const
    {
        return m_layout;
    }

    void OpenGLVertexBuffer::SetLayout(const BufferLayout &layout)
    {
        m_layout = layout;
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32 *indices, uint32 count)
        : m_count(count)
    {
        glCreateBuffers(1, &m_rendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_rendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID); 
    }

    void OpenGLIndexBuffer::Unbind() const
    {
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32 OpenGLIndexBuffer::GetCount() const
    {
        return m_count;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_rendererID);
    }

    OpenGLVertexArray::OpenGLVertexArray(const SharedPtr<VertexBuffer> &vertexBuffer, const SharedPtr<IndexBuffer> &indexBuffer)
    {
        glCreateVertexArrays(1, &m_rendererID);
        SetVertexBuffer(vertexBuffer);
        SetIndexBuffer(indexBuffer);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_rendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::SetVertexBuffer(SharedPtr<VertexBuffer> vertexBuffer)
    {
        const BufferLayout& layout = vertexBuffer->GetLayout();
        for(uint8 index = 0; index < layout.GetSize(); ++index)
        {
            auto& element = layout.GetElement(index);
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
        m_vertexBuffer = vertexBuffer;
    }

    void OpenGLVertexArray::SetIndexBuffer(SharedPtr<IndexBuffer> indexBuffer)
    {
        m_indexBuffer = indexBuffer;
    }
}
