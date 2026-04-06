#pragma once

#include "Event/Event.h"

namespace axiom
{
    class WindowEvent : public Event
    {
    public:
        static inline const String RESIZE = "WindowEvent::Resize";

        WindowEvent(StringView type) : Event(type) {}

        uint16 m_oldWidth;
        uint16 m_oldHeight;
        uint16 m_newWidth;
        uint16 m_newHeight;
    };
}