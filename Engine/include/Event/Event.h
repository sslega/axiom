#pragma once

#include "Core/Types.h"
#include <functional>

namespace axiom
{
    #define AX_EVENT_FN(fn) [this](const axiom::Event& e) { fn(e); }

    class Event
    {
    public:
        Event(StringView type);
        virtual ~Event() = default;
        const StringView GetType() const { return m_type; }
        const bool GetCancelable() const { return m_cancelable; }

    private:
        String m_type;
        bool m_cancelable;
        void* m_target;
        
        friend class EventDispatcher;
    };

    struct EventListener
    {
        uint32 id;
        String type;
        std::function<void(const Event&)> fn;
    };

    class EventDispatchable
    {
    public:
        virtual EventListener AddEventListener(StringView type, std::function<void(const Event&)> fn) = 0;
        virtual void RemoveEventListener(const EventListener& listener) = 0;
        virtual bool WillTrigger(StringView type) = 0;
        virtual void DispatchEvent(const Event& event) = 0;
    };

    class EventDispatcher : public EventDispatchable
    {
    public:
        EventDispatcher();
        EventDispatcher(EventDispatchable& eventDispatchable);
        virtual EventListener AddEventListener(StringView type, std::function<void(const Event&)> fn) override;
        virtual void RemoveEventListener(const EventListener& listener) override;
        virtual bool WillTrigger(StringView type) override;
        virtual void DispatchEvent(const Event& event) override;

    private:
        uint32 m_nextId = 0;
        EventDispatchable& m_target;
        StringMap<Vector<EventListener>> m_listeners;
    };
}