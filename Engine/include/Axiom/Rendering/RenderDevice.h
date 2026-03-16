#pragma once

#include "Axiom/Core/Types.h"
#include "Axiom/Core/Memory.h"
#include "Axiom/Platform/ApplicationWindow.h" // for IApplicationWindow
#include <memory>

namespace axiom
{
    enum class RenderAPI
    {
        None,
        OpenGL,
        Vulkan,
        DirectX11,
        DirectX12,
    };

    class IRenderDevice
    {
    public:
        virtual ~IRenderDevice() = default;

        virtual bool Initialize(void* nativeWindowHandle) = 0;
        virtual void Shutdown() = 0;
        virtual void BeginFrame() = 0;
        virtual void Draw() = 0;
        virtual void EndFrame() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;
    };

    UniquePtr<IRenderDevice> CreateRenderDevice(RenderAPI api);
}
