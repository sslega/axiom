#pragma once

#include "../Core/Types.h"
#include "../Core/Memory.h"
#include "../Platform/ApplicationWindow.h" // for IApplicationWindow
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

    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual bool Initialize(void* nativeWindowHandle) = 0;
        virtual void Shutdown() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;
    };

    UniquePtr<IRenderer> CreateRenderer(RenderAPI api);
}
