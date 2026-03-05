#pragma once

#include "Rendering/RenderDevice.h"

namespace axiom
{
    class OpenGLRenderDevice : public IRenderDevice
    {
    public:
        ~OpenGLRenderDevice() override = default;

        bool Initialize(void* nativeWindowHandle) override;
        void Shutdown() override;
        void BeginFrame() override;
        void EndFrame() override;
        void Resize(uint32 width, uint32 height) override;

    private:
        // simple triangle data
        uint32_t m_VAO = 0;
        uint32_t m_VBO = 0;
        uint32_t m_ColorVBO = 0;
        uint32_t m_ShaderProgram = 0;

        bool SetupTriangle();
        bool CompileShaders();
    };
}
