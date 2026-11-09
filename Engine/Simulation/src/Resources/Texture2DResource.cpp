#include "Resources/Texture2DResource.h"

namespace axiom
{
    Texture2DResource::Texture2DResource(uint32 width, uint32 height, Vector<uint8> data)
    : m_width(width)
    , m_height(height)
    , m_data(std::move(data))
    {
    }

    uint32 Texture2DResource::GetWidth() const
    {
        return m_width;
    }

    uint32 Texture2DResource::GetHeight() const
    {
        return m_height;
    }

    const uint8* Texture2DResource::GetData() const
    {
        return m_data.data();
    }
}
