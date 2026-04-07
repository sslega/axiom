#include "Platform/ApplicationWindow.h"
#include "Platform/Windows/Win32Window.h"
#include "Platform/GLFW/GLFWWindow.h"

namespace axiom
{
    UniquePtr<ApplicationWindow> ApplicationWindow::Create(const AppWindowConfig& config)
    {
        switch (config.backend)
        {
            case ApplicationWindowBackend::Win32:
                return MakeUnique<Win32Window>(config);
            case ApplicationWindowBackend::GLFW:
                return MakeUnique<GLFWWindow>(config);
            default:
                return nullptr;
        }
    }
}
