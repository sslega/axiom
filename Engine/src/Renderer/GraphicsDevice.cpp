#include "Renderer/GraphicsDevice.h"
#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/ApplicationWindow.h"
#include "Core/Assert.h"


namespace axiom
{
    UniquePtr<GraphicsDevice> GraphicsDevice::Create(GraphicsDevice::API api, const ApplicationWindow& window)
    {
        switch (api)
        {
            case GraphicsDevice::API::OpenGL: return MakeUnique<OpenGLGraphicsDevice>(window);
        }
        AX_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;  
    }
}
