#include "Axiom/Rendering/RenderDevice.h"
#include "Axiom/Rendering/GLRenderDevice.h"

namespace axiom
{
    UniquePtr<IRenderDevice> CreateRenderDevice(RenderAPI api)
    {
        switch (api)
        {
            case RenderAPI::OpenGL:
                return MakeUnique<GLRenderDevice>();
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
