#pragma once

#include "Input/Input.h"
#include "GLFW/glfw3.h"

namespace axiom
{
    class ApplicationWindow;

    class GLFWInput : public Input
    {
    public:
        GLFWInput(const ApplicationWindow& window);

    protected:
        bool  IsKeyPressedInternal(KeyCode key) const override;
        bool  IsMouseButtonPressedInternal(MouseCode button) const override;
        Vec2  GetMousePositionInternal() const override;
        float GetScrollDeltaInternal() const override;
        void SetCursorLockedInternal(bool locked) override;
    private:
        mutable float m_scrollDelta = 0.0f;
        GLFWwindow* m_nativeWindow;
    };
}
