#pragma once
#include "Math/Matrix4.h"
#include "Math/Vector.h"

namespace axiom
{
    class Scene;
    class FrameBuffer;
    class PostProcessBase;

    /// One render request: draw the scene from this camera into this framebuffer, then run these effects.
    ///
    /// Built and submitted every frame through RenderSubsystem::SubmitView(). In the editor, Viewport fills it in
    /// and ViewportPanel submits it. The effect pointers don't own anything; whoever fills in the View keeps them alive.
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