#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector.h"

namespace axiom
{
    class Scene;
    class FrameBuffer;

    struct View
    {
        Matrix4 viewProjection;
        Vec3 cameraPosition;
        SharedPtr<FrameBuffer> renderTarget;
        String debugName;
        uint32 width {0};
        uint32 height {0};
        Vec4 clearColor {0, 0, 0, 1};
    };
}