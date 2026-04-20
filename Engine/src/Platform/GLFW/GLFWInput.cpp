#include "Platform/GLFW/GLFWInput.h"
#include "Platform/GLFW/GLFWWindow.h"
#include "Platform/ApplicationWindow.h"
#include <glfw/glfw3.h>
#include "GLFWInput.h"

namespace axiom
{
    GLFWInput::GLFWInput(const ApplicationWindow &window)
    : Input(window)
    {
    }

    bool GLFWInput::IsKeyPressedInternal(KeyCode key) const
    {
        auto* window = static_cast<GLFWwindow*>(m_window.GetNativeWindow());
        return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
    }

    bool GLFWInput::IsMouseButtonPressedInternal(MouseCode button) const
    {
        auto* window = static_cast<GLFWwindow*>(m_window.GetNativeWindow());
        return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
    }

    Vec2 GLFWInput::GetMousePositionInternal() const
    {
        auto* window = static_cast<GLFWwindow*>(m_window.GetNativeWindow());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { (float)x, (float)y };
    }

    float GLFWInput::GetScrollDeltaInternal() const
    {
        float delta = m_scrollDelta;
        m_scrollDelta = 0.0f;  // consume on read
        return delta;
    }
}
