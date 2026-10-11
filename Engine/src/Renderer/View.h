#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector.h"

namespace axiom
{
    class Scene;
    class FrameBuffer;
    class PostProcessBase;

    struct View
    {
        Matrix4 viewProjection;
        Vec3 cameraPosition;
        SharedPtr<FrameBuffer> renderTarget;
        String name;
        uint32 width {0};
        uint32 height {0};
        Vec4 clearColor {0, 0, 0, 1};
        Vector<PostProcessBase*> postProcesses;
    };
}