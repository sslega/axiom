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

    class EventDispatchable
    {
    public:
        virtual ~EventDispatchable() = default;

        template<typename T, typename Obj>
        EventListener AddEventListener(void(Obj::*method)(const T&), Obj* instance)
        {
            return AddEventListener(typeid(T), [instance, method](const Event& e) {
                (instance->*method)(static_cast<const T&>(e));
            });
        }

        template<typename T>
        EventListener AddEventListener(std::function<void(const T&)> fn)
        {
            return AddEventListener(typeid(T), [fn](const Event& e) {
                fn(static_cast<const T&>(e));
            });
        }

        virtual EventListener AddEventListener(std::type_index type, std::function<void(const Event&)> fn) = 0;
        virtual void RemoveEventListener(const EventListener& listener) = 0;
        virtual bool WillTrigger(std::type_index type) = 0;
        virtual void DispatchEvent(const Event& event) = 0;
    };

    class EventDispatcher : public EventDispatchable
    {
    public:
        EventDispatcher();
        EventDispatcher(EventDispatchable& eventDispatchable);

        using EventDispatchable::AddEventListener;
        virtual EventListener AddEventListener(std::type_index type, std::function<void(const Event&)> fn) override;
        virtual void RemoveEventListener(const EventListener& listener) override;
        virtual bool WillTrigger(std::type_index type) override;
        virtual void DispatchEvent(const Event& event) override;

    private:
        uint32 m_nextId = 0;
        EventDispatchable& m_target;
        TypeMap<Vector<EventListener>> m_listeners;
    };
}
