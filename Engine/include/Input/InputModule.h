#pragma once

#include "Core/Types.h"
#include "Core/ApplicationModule.h"
#include "Input/Keyboard.h"

namespace axiom
{
    class Input;

    class InputModule : public ApplicationModule
    {
    public:
        InputModule(Application& application);
        ~InputModule() = default;
        bool IsKeyPressed(KeyCode key) const;
    private:
        UniquePtr<Input> m_input;
    };
}