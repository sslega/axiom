#pragma once

#include "Input/Input.h"

namespace axiom
{
    class ApplicationWindow;

    class GLFWInput : public Input
    {
    public:
        GLFWInput(const ApplicationWindow& window);

        virtual bool IsKeyPressed(KeyCode key) const override;
    };
}
