#include "Renderer/RenderResourceFactory.h"
#include "Platform/OpenGL/OpenGLRenderResourceFactory.h"
#include "Core/Assert.h"

namespace axiom
{
    UniquePtr<RenderResourceFactory> RenderResourceFactory::Create(GraphicsDevice::API api)
    {
        switch (api)
        {
            case GraphicsDevice::API::OpenGL: return MakeUnique<OpenGLRenderResourceFactory>();
        }
        AX_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }
}
