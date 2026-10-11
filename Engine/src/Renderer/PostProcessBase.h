#pragma once

#include "Core/Types.h"

namespace axiom
{
    class FrameBuffer;
    class RenderSubsystem;

    class PostProcessBase
    {
    public:
        bool enabled {true};

        virtual void Execute(RenderSubsystem& renderer, FrameBuffer& source, FrameBuffer& target) = 0;
    };
}