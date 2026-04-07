#pragma once

#include "Core/Types.h"
#include "Input/Keyboard.h"

namespace axiom
{
    class ApplicationWindow;

    class Input
    {
    public:
        Input(const ApplicationWindow& window);

        static UniquePtr<Input> Create(const ApplicationWindow& window);

        virtual bool IsKeyPressed(KeyCode key) const = 0;
    protected:
        const ApplicationWindow& m_window;
    };
}