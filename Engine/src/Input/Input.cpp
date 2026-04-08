#include "Input/Input.h"
#include "Core/Types.h"
#include "Platform/ApplicationWindow.h"
#include "Platform/GLFW/GLFWInput.h"
#include "Core/Assert.h"

namespace axiom
{
    Input* Input::s_instance = nullptr;

    axiom::Input::Input(const ApplicationWindow& window)
    : m_window(window)
    {
        AX_ASSERT(s_instance == nullptr, "Input instance already exists");
        s_instance = this;
    }

    UniquePtr<Input> axiom::Input::Create(const ApplicationWindow& window)
    {
    
        switch (window.GetBackend()) 
        {
            case ApplicationWindowBackend::GLFW: return MakeUnique<GLFWInput>(window);
        }
        AX_ASSERT(false, "unknown window backend");
        return nullptr;
    }
}

bool axiom::Input::IsKeyPressed(KeyCode key)
{
    return s_instance->IsKeyPressed_(key);
}
