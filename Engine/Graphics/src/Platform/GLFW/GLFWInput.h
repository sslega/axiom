#pragma once

#include "Input/Input.h"
#include "GLFW/glfw3.h"

namespace axiom
{
    class ApplicationWindow;

    class GLFWInput : public IInput
    {
    public:
        GLFWInput(const ApplicationWindow& window);

    protected:
        bool  IsKeyPressed(KeyCode key) const override;
        bool  IsMouseButtonPressed(MouseCode button) const override;
        Vec2  GetMousePosition() const override;
        float GetScrollDelta() const override;
        void SetCursorLocked(bool locked) override;
    private:
        mutable float m_scrollDelta = 0.0f;
        GLFWwindow* m_nativeWindow;
    };
}
