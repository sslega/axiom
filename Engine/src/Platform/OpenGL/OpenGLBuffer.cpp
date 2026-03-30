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

    const BufferLayout &OpenGLVertexBuffer::GetLayout() const
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

}
