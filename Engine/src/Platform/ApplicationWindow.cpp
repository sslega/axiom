#include "Platform/ApplicationWindow.h"
#include "Platform/Windows/Win32Window.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Core/Assert.h"

namespace axiom
{
    ApplicationWindow::ApplicationWindow(AppWindowConfig config)
    {
        m_windowConfig = config;
    }


    UniquePtr<ApplicationWindow> ApplicationWindow::Create(const AppWindowConfig& config)
    {
        switch (config.backend)
        {
            case ApplicationWindowBackend::Win32:
                return MakeUnique<Win32Window>(config);
            case ApplicationWindowBackend::GLFW:
                return MakeUnique<GLFWWindow>(config);
        }
        AX_ASSERT(false, "unknown window backend");
        return nullptr;
    }
}

