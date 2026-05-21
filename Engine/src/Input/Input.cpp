#include "Input/Input.h"
#include "Platform/ApplicationWindow.h"
#include "Platform/GLFW/GLFWInput.h"
#include "Core/Assert.h"

namespace axiom
{
    IInput* Input::s_instance = nullptr;

    UniquePtr<IInput> IInput::Create(const ApplicationWindow& window)
    {
        switch (window.GetBackend())
        {
            case ApplicationWindowBackend::GLFW: return MakeUnique<GLFWInput>(window);
        }
        AX_ASSERT(false, "unknown window backend");
        return nullptr;
    }
}
