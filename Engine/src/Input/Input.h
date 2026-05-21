#pragma once

#include "Core/Types.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Core/Application.h"
#include "Core/Global.h"

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
            return GInput->IsKeyPressedInternal(key);
        }

        static bool IsMouseButtonPressed(MouseCode button)
        {
            return GInput->IsMouseButtonPressedInternal(button);
        }

        static Vec2 GetMousePosition()
        {
            return GInput->GetMousePositionInternal();
        }

        static void SetCursorLocked(bool locked)
        {
            GInput->SetCursorLockedInternal(locked);
        }

        static float GetScrollDelta()
        {
            return GInput->GetScrollDeltaInternal();
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