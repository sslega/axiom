#pragma once

#include "Core/Types.h"

namespace axiom
{
    class FrameBuffer;
    class RenderSubsystem;
    struct View;

    struct PostProcessContext
    {
        RenderSubsystem& renderer;
        const View& view;
        FrameBuffer& source;
        
        FrameBuffer* customMask;
    };

    /// Base class of a full-screen effect attached to a Viewport. Implement Execute().
    ///
    /// The contract: an early-out leaves the image untouched, the effect writes its result back itself,
    /// and it releases every temporary framebuffer it acquires.
    ///
    /// @code{.cpp}
    /// viewport->AddPostProcess<OutlinePostProcess>(outlineColor);
    /// @endcode
    class PostProcessBase
    {
    public:
        bool enabled {true};

        virtual void Execute(const PostProcessContext& context) = 0;
    };
}