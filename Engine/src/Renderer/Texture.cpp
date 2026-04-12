#include "Renderer/Texture.h"
#include "Core/Application.h"
#include "Core/Types.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace axiom
{
    SharedPtr<Texture2D> axiom::Texture2D::Create(const String& path)
    {
        GraphicsDevice::API api = Application::Get().GetRenderAPI();
        switch(api)
        {
            case GraphicsDevice::API::OpenGL: return MakeShared<OpenGLTexture2D>(path);
        }
        AX_ASSERT(false, "Unknown render api!");
        return nullptr;
    }
}