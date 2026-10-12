#pragma once

#include "Core/Types.h"

namespace axiom
{
    using RenderProxyID = uint32;
    static constexpr RenderProxyID InvalidProxyID = 0;

    enum class RenderFlags : uint32
    {
        None = 0,
        CustomMask = 1 << 0,
        // Wireframe = 1 << 1,
    };

    inline RenderFlags operator|(RenderFlags a, RenderFlags b)
    {
        using T = std::underlying_type_t<RenderFlags>;
        return static_cast<RenderFlags>(static_cast<T>(a) | static_cast<T>(b));
    }

    inline RenderFlags operator&(RenderFlags a, RenderFlags b)
    {
        using T = std::underlying_type_t<RenderFlags>;
        return static_cast<RenderFlags>(static_cast<T>(a) & static_cast<T>(b));
    }

    inline RenderFlags operator~(RenderFlags a)
    {
        using T = std::underlying_type_t<RenderFlags>;
        return static_cast<RenderFlags>(~static_cast<T>(a));
    }

    inline RenderFlags& operator|=(RenderFlags& a, RenderFlags b) { return a = a | b; }
    inline RenderFlags& operator&=(RenderFlags& a, RenderFlags b) { return a = a & b; }

    inline bool HasAnyFlag(RenderFlags flags, RenderFlags mask)
    {
        return (flags & mask) != RenderFlags::None;
    }
}
