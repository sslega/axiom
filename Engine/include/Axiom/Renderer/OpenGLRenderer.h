#pragma once

#include "axiom/Renderer/Renderer.h"

namespace axiom
{
    class OpenGLRenderer : public IRenderer
    {
    public:
        ~OpenGLRenderer() override = default;

        bool Initialize(void* nativeWindowHandle) override;
        void Shutdown() override;
        void BeginFrame() override;
        void EndFrame() override;
        void Resize(uint32 width, uint32 height) override;
    };
}
