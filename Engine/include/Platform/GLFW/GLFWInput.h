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
        virtual bool IsKeyPressed_(KeyCode key) override;
        
    };
}
