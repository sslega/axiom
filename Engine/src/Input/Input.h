#pragma once

#include "Core/Types.h"
#include "Input/Keyboard.h"
#include "Core/Application.h"

namespace axiom
{
    class ApplicationWindow;

    class Input
    {
    public:
        Input(const ApplicationWindow& window);

        static UniquePtr<Input> Create(const ApplicationWindow& window);
        static bool IsKeyPressed(KeyCode key)
        {
            return Application::Get().m_input->IsKeyPressedInternal(key);
        }
    protected:
        const ApplicationWindow& m_window;
        virtual bool IsKeyPressedInternal(KeyCode key) const = 0;
    };
}