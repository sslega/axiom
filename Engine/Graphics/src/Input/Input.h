#pragma once

#include "Core/Types.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Math/Vector.h"

namespace axiom
{
    class ApplicationWindow;

    class IInput
    {
    public:
        virtual ~IInput() = default;
        static UniquePtr<IInput> Create(const ApplicationWindow& window);

        virtual bool IsKeyPressed(KeyCode key) const = 0;
        virtual Vec2 GetMousePosition() const = 0;
        virtual bool  IsMouseButtonPressed(MouseCode button) const = 0;
        virtual float GetScrollDelta() const = 0;
        virtual void SetCursorLocked(bool locked) = 0;
    };

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key)
        {
            return s_instance->IsKeyPressed(key);
        }

        static bool IsMouseButtonPressed(MouseCode button)
        {
            return s_instance->IsMouseButtonPressed(button);
        }

        static Vec2 GetMousePosition()
        {
            return s_instance->GetMousePosition();
        }

        static void SetCursorLocked(bool locked)
        {
            s_instance->SetCursorLocked(locked);
        }

        static float GetScrollDelta()
        {
            return s_instance->GetScrollDelta();
        }

    // protected:
    //     const ApplicationWindow& m_window;

    private:
        static void SetInstance(IInput* instance) { s_instance = instance; }
        static IInput* s_instance;

        friend class Application;
    };
}