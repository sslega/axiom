#include "Input/InputModule.h"

#include "Input/Input.h"
#include "Core/Application.h"

namespace axiom
{
    InputModule::InputModule(Application &application)
    : ApplicationModule(application)
    {
        m_input = Input::Create(application.GetApplicationWindow());
    }
}

bool axiom::InputModule::IsKeyPressed(KeyCode key) const
{
    return m_input->IsKeyPressed(key);
}
