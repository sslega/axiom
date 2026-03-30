#pragma once

#include "Renderer/Buffer.h"
#include <glad/glad.h>

namespace axiom
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Bool : return GL_BOOL;
            case ShaderDataType::Float : return GL_FLOAT;
            case ShaderDataType::Float2 : return GL_FLOAT;
            case ShaderDataType::Float3 : return GL_FLOAT;
            case ShaderDataType::Float4 : return GL_FLOAT;
            case ShaderDataType::Int : return GL_INT;
            case ShaderDataType::Int2 : return GL_INT;
            case ShaderDataType::Int3 : return GL_INT;
            case ShaderDataType::Int4 : return GL_INT;
            case ShaderDataType::Mat3 : return GL_FLOAT;
            case ShaderDataType::Mat4 : return GL_FLOAT;
        }
        AX_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32 size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;
        virtual const BufferLayout& GetLayout() const override;
        virtual void SetLayout(const BufferLayout& layout) override;
    private:
        uint32 m_rendererID;
        BufferLayout m_layout;
    };

     class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32* indices, uint32 count);
        virtual ~OpenGLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;
        virtual uint32 GetCount() const;
    private:
        uint32 m_rendererID;
        uint32 m_count;
    };
}