#include "Platform/GLFW/GLFWInput.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/ApplicationWindow.h"
#include "GLFWInput.h"

namespace axiom
{
    GLFWInput::GLFWInput(const ApplicationWindow& window)
    {
        m_nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
    }

    bool GLFWInput::IsKeyPressed(KeyCode key) const
    {
        return glfwGetKey(m_nativeWindow, static_cast<int>(key)) == GLFW_PRESS;
    }

    bool GLFWInput::IsMouseButtonPressed(MouseCode button) const
    {
        return glfwGetMouseButton(m_nativeWindow, static_cast<int>(button)) == GLFW_PRESS;
    }

    Vec2 GLFWInput::GetMousePosition() const
    {
        double x, y;
        glfwGetCursorPos(m_nativeWindow, &x, &y);
        return { (float)x, (float)y };
    }

    float GLFWInput::GetScrollDelta() const
    {
        float delta = m_scrollDelta;
        m_scrollDelta = 0.0f;  // consume on read
        return delta;
    }
    
    void GLFWInput::SetCursorLocked(bool locked)
    {
        glfwSetInputMode(m_nativeWindow, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}
