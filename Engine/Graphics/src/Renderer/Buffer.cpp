#include "Renderer/Buffer.h"
namespace axiom
{
    BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_elements(elements)
    {
        CalculateOffsetAndStride();
    }
    
    void BufferLayout::CalculateOffsetAndStride()
    {
        uint32 offset = 0;
        m_stride = 0;
        for(auto& element : m_elements)
        {
            element.offset = offset;
            offset += element.size;
            m_stride += element.size;
        }
    }
}