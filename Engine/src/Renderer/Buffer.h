#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"

namespace axiom
{
    enum class ShaderDataType
    {
        None,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32 ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1;
        }
        AX_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    struct BufferElement
    {
        String name;
        ShaderDataType type;
        uint32 size;
        uint32 offset;
        bool normalized;
        BufferElement(){}
        BufferElement(ShaderDataType _type, const String& _name)
        : name(_name)
        , type(_type)
        , size(ShaderDataTypeSize(_type))
        , offset(0)
        , normalized(false)
        {}

        uint32 GetComponentCount() const
        {
            switch(type)
            {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;
                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;
                case ShaderDataType::Bool: return 1;
            }
            AX_ASSERT(false, "Unknown type!");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout(){};
        BufferLayout(const std::initializer_list<BufferElement>& elements);
        inline const Vector<BufferElement>& GetElements() const { return m_elements; }
        inline const BufferElement& GetElement(int index) const { return m_elements[index]; }
        inline const uint8 GetSize() const { return m_elements.size(); };
        inline const uint32 GetStride() const { return m_stride; };
    private:
        Vector<BufferElement> m_elements;
        uint32 m_stride = 0;
        void CalculateOffsetAndStride();
    };

    class Buffer
    {
    public:
        virtual ~Buffer() {};
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() {}
        
        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;
        virtual void SetData(const void* data, uint32 size) = 0;
    };

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual uint32 GetCount() const = 0;
        virtual void SetData(const void* data, uint32 count) = 0;
    };

    // TODO: this is purely OpenGL concept - remove it later
    class VertexArray : public Buffer
    {
    public:
        virtual ~VertexArray() {}
        
        virtual void SetVertexBuffer(SharedPtr<VertexBuffer> vertexBuffer) = 0;
        virtual void SetIndexBuffer(SharedPtr<IndexBuffer> indexBuffer) = 0;

        virtual SharedPtr<VertexBuffer> GetVertexBuffer() const = 0;
        virtual SharedPtr<IndexBuffer> GetIndexBuffer() const = 0;
    };
}