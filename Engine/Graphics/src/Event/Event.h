#pragma once

#include "Core/Types.h"
#include <functional>

namespace axiom
{
    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;
        const bool GetCancelable() const { return m_cancelable; }

    private:
        bool m_cancelable = true;
        void* m_target = nullptr;

        friend class EventDispatcher;
    };

    struct EventListener
    {
        uint32 id;
        std::type_index type;
        std::function<void(const Event&)> fn;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher();
        EventDispatcher(EventDispatcher& target);

        template<typename T, typename Obj>
        EventListener AddEventListener(void(Obj::*method)(const T&), Obj* instance)
        {
            return AddEventListenerInternal(typeid(T), [instance, method](const Event& e) {
                (instance->*method)(static_cast<const T&>(e));
            });
        }

        void RemoveEventListener(const EventListener& listener);
        bool WillTrigger(std::type_index type);
        void DispatchEvent(const Event& event);

    private:
        EventListener AddEventListenerInternal(std::type_index type, std::function<void(const Event&)> fn);

        uint32 m_nextId = 0;
        EventDispatcher* m_target;
        TypeMap<Vector<EventListener>> m_listeners;
    };
}
