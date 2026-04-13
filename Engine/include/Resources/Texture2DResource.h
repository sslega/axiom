#pragma once

#include "Core/Types.h"

namespace axiom
{
    class Texture2DResource
    {
    public:
        Texture2DResource(uint32 width, uint32 height, Vector<uint8> data);

        uint32 GetWidth() const;
        uint32 GetHeight() const;
        const uint8* GetData() const;

    private:
        uint32 m_width;
        uint32 m_height;
        Vector<uint8> m_data; // RGBA decoded pixels
    };
}
