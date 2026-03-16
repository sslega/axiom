#pragma once

#include "Axiom/Rendering/RenderDevice.h"

namespace axiom
{
    class GLRenderDevice : public IRenderDevice
    {
    public:
        ~GLRenderDevice() override = default;

        bool Initialize(void* nativeWindowHandle) override;
        void Shutdown() override;
        void Resize(uint32 width, uint32 height) override;
        
        void BeginFrame() override;
        void Draw() override;
        void EndFrame() override;

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
