#pragma once

#include "Event/Event.h"

namespace axiom
{
    class KeyboardEvent : public Event
    {

    public:
        enum Action
        {
            RELEASE,
            PRESS,
            REPEAT
        };

        uint16 m_keyCode;
        uint16 m_charCode;
        Action m_action;
        bool m_altKey;
        bool m_controlKey;
        bool m_shiftKey;
    };
}