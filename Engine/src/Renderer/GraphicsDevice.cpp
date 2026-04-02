#include "Renderer/GraphicsDevice.h"
#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Core/Assert.h"

namespace axiom
{
    UniquePtr<GraphicsDevice> GraphicsDevice::Create(GraphicsDevice::API api)
    {
        switch (api)
        {
            case GraphicsDevice::API::OpenGL: return MakeUnique<OpenGLGraphicsDevice>();
        }
        AX_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;  
    }
}