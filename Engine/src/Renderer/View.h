#pragma once
#include "Math/Matrix4.h"

namespace axiom
{
    class Scene;
    class FrameBuffer;

    enum class PassType 
    {
        DepthOnly,
        Full
    };

    struct View
    {
        Scene* scene {nullptr};
        Matrix4 viewProjection;
        SharedPtr<FrameBuffer> renderTarget;
        PassType passType {PassType::Full};
        String debugName;
    };
}