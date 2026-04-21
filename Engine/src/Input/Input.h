#pragma once

#include "Core/Types.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
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

        static bool IsMouseButtonPressed(MouseCode button)
        {
            return Application::Get().m_input->IsMouseButtonPressedInternal(button);
        }

        static Vec2 GetMousePosition()
        {
            return Application::Get().m_input->GetMousePositionInternal();
        }

        static void SetCursorLocked(bool locked)
        {
             Application::Get().m_input->SetCursorLockedInternal(locked);
        }

        static float GetScrollDelta()
        {
            return Application::Get().m_input->GetScrollDeltaInternal();
        }

    protected:
        const ApplicationWindow& m_window;
        virtual bool IsKeyPressedInternal(KeyCode key) const = 0;
        virtual Vec2 GetMousePositionInternal() const = 0;
        virtual bool  IsMouseButtonPressedInternal(MouseCode button) const = 0;
        virtual float GetScrollDeltaInternal() const = 0;
        virtual void SetCursorLockedInternal(bool locked) = 0;
    };
}