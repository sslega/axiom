#include "Rendering/RenderDevice.h"
#include "Rendering/OpenGLRenderer.h"

namespace axiom
{
    UniquePtr<IRenderDevice> CreateRenderDevice(RenderAPI api)
    {
        switch (api)
        {
            case RenderAPI::OpenGL:
                return MakeUnique<OpenGLRenderDevice>();
            case RenderAPI::Vulkan:
                return nullptr;
            case RenderAPI::DirectX11:
                return nullptr;    
            case RenderAPI::DirectX12:
                return nullptr;
            case RenderAPI::None:
            default:
                return nullptr;
        }
    }
}
