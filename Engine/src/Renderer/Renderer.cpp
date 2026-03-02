#include "axiom/Renderer/Renderer.h"
#include "axiom/Renderer/OpenGLRenderer.h"

namespace axiom
{
    UniquePtr<IRenderer> CreateRenderer(RenderAPI api)
    {
        switch (api)
        {
            case RenderAPI::OpenGL:
                return MakeUnique<OpenGLRenderer>();
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
