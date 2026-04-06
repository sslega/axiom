#pragma once

#include "Event/Event.h"

namespace axiom
{
    class KeyboardEvent : public Event
    {
    public:
        static inline const String KEY_DOWN = "KeyboardEvent::KeyDown";
        static inline const String KEY_UP = "KeyboardEvent::KeyUp";

        uint16 m_keyCode;
        uint16 m_charCode;
        bool m_altKey;
        bool m_controlKey;
        bool m_shiftKey;
    };
}