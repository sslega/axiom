#include "Platform/GLFW/GLFWInput.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/ApplicationWindow.h"
#include <glfw/glfw3.h>

namespace axiom
{
    GLFWInput::GLFWInput(const ApplicationWindow &window)
    : Input(window)
    {
    }

    bool GLFWInput::IsKeyPressed_(KeyCode key)
    {
        auto* window = static_cast<GLFWwindow*>(m_window.GetNativeWindow());
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }
}

