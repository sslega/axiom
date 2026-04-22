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
            return Application::Get().GetInput().IsKeyPressedInternal(key);
        }

        static bool IsMouseButtonPressed(MouseCode button)
        {
            return Application::Get().GetInput().IsMouseButtonPressedInternal(button);
        }

        static Vec2 GetMousePosition()
        {
            return Application::Get().GetInput().GetMousePositionInternal();
        }

        static void SetCursorLocked(bool locked)
        {
             Application::Get().GetInput().SetCursorLockedInternal(locked);
        }

        static float GetScrollDelta()
        {
            return Application::Get().GetInput().GetScrollDeltaInternal();
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