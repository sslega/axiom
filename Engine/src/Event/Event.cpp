#include "Event/Event.h"

namespace axiom
{
    Event::Event(StringView type)
    : m_type(type)
    {
    }

    axiom::EventDispatcher::EventDispatcher()
    : m_target(*this)
    {
    }

    EventDispatcher::EventDispatcher(EventDispatchable &eventDispatchable)
    : m_target(eventDispatchable)
    {
    }
    
    EventListener EventDispatcher::AddEventListener(StringView type, std::function<void(const Event&)> fn)
    {
        EventListener listener { m_nextId++, String(type), fn };
        m_listeners[String(type)].push_back(listener);
        return listener;
    }

    void EventDispatcher::RemoveEventListener(const EventListener& listener)
    {
        auto it = m_listeners.find(listener.type);
        if (it == m_listeners.end()) return;

        auto& vec = it->second;
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [&listener](const EventListener& e) { return e.id == listener.id; }),
            vec.end());
    }

    bool EventDispatcher::WillTrigger(StringView type)
    {
        return false;
    }

    void EventDispatcher::DispatchEvent(const Event& event)
    {
        auto it = m_listeners.find(String(event.GetType()));
        if (it == m_listeners.end()) return;

        const_cast<Event&>(event).m_target = &m_target;

        for (const auto& entry : it->second)
            entry.fn(event);
    }

}