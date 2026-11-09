#pragma once

#include "Event/Event.h"

namespace axiom
{
    class WindowResizeEvent : public Event
    {
    public:
        uint16 m_oldWidth  = 0;
        uint16 m_oldHeight = 0;
        uint16 m_newWidth  = 0;
        uint16 m_newHeight = 0;
    };

    class WindowCloseEvent : public Event {};
}
