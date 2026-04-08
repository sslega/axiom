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
        static bool IsKeyPressed(KeyCode key);
    
    protected:
        const ApplicationWindow& m_window;
        static Input* s_instance;
        virtual bool IsKeyPressed_(KeyCode key) = 0;
    };

    // class Input
    // {
    // public:
    //     Input(const ApplicationWindow& window);

    //     static UniquePtr<Input> Create(const ApplicationWindow& window);

    //     virtual bool IsKeyPressed(KeyCode key) const = 0;
    // protected:
    //     const ApplicationWindow& m_window;
    // };

    // inline bool IsKeyPressed(KeyCode key)
    // {
    //     return Application::Get().GetModule<InputModule>()->IsKeyPressed(key);
    // }
}