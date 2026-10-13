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

    class PostProcessBase
    {
    public:
        bool enabled {true};

        virtual void Execute(const PostProcessContext& context) = 0;
    };
}