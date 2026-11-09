#include "Event/Event.h"

namespace axiom
{
    EventDispatcher::EventDispatcher()
    : m_target(nullptr)
    {
    }

    EventDispatcher::EventDispatcher(EventDispatcher& target)
    : m_target(&target)
    {
    }

    EventListener EventDispatcher::AddEventListenerInternal(std::type_index type, std::function<void(const Event&)> fn)
    {
        EventListener listener { m_nextId++, type, fn };
        m_listeners[type].push_back(listener);
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

    bool EventDispatcher::WillTrigger(std::type_index type)
    {
        auto it = m_listeners.find(type);
        return it != m_listeners.end() && !it->second.empty();
    }

    void EventDispatcher::DispatchEvent(const Event& event)
    {
        auto it = m_listeners.find(typeid(event));
        if (it == m_listeners.end()) return;

        EventDispatcher* target = m_target ? m_target : this;
        const_cast<Event&>(event).m_target = target;

        for (const auto& entry : it->second)
            entry.fn(event);
    }
}
