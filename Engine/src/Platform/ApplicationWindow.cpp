#include "axiom/Platform/ApplicationWindow.h"
#include "axiom/Platform/Windows/Win32Window.h"

namespace axiom
{
    std::unique_ptr<IApplicationWindow> IApplicationWindow::Create(const ApplicationWindowDesc &desc)
    {
        // Right now we only have a Win32 implementation, but in the future we can add more platform-specific implementations here
        return std::make_unique<Win32Window>(desc);
    }
}