#pragma once
#include "Core/Types.h"
#include "Core/Object.h"

namespace axiom
{

    struct IObjectResolvable {
        virtual void Resolve(const StringMap<SharedPtr<Object>>& registry) = 0;
        virtual ~IObjectResolvable() = default;
    };


    template<typename T>
    class ObjectHandle : public IObjectResolvable 
    {
    public:
        void SetID(const String& id) { m_id = id; };
        const String& GetID() const { return m_id; };
        void Set(SharedPtr<T> object) { m_object = object; };
        SharedPtr<T> Get() const { return m_object; };

        virtual void Resolve(const StringMap<SharedPtr<Object>>& objectsRegistry) override
        {
            auto it = objectsRegistry.find(m_id);
            if (it != objectsRegistry.end())
            {
                m_object = std::dynamic_pointer_cast<T>(it->second);
            }
        };
    private:
        SharedPtr<T> m_object;
        String m_id;
    };
    
} // namespace axiom
