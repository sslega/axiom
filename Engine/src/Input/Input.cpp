#include "Input/Input.h"
#include "Core/Types.h"
#include "Platform/ApplicationWindow.h"
#include "Platform/GLFW/GLFWInput.h"
#include "Core/Assert.h"

namespace axiom
{
    axiom::Input::Input(const ApplicationWindow& window)
    : m_window(window)
    {
    }

    UniquePtr<Input> axiom::Input::Create(const ApplicationWindow &window)
    {
        switch (window.GetBackend()) 
        {
            case ApplicationWindowBackend::GLFW: return MakeUnique<GLFWInput>(window);
        }
        AX_ASSERT(false, "unknown window backend");
        return nullptr;
    }
}