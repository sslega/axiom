#pragma once

#include "Input/Input.h"

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
    private:
        mutable float m_scrollDelta = 0.0f;
    };
}
