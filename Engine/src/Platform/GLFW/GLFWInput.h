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
        virtual bool IsKeyPressedInternal(KeyCode key) const override;
        
    };
}
