#pragma once
#include "Core/Types.h"
#include "Scene/Component.h"

namespace axiom
{

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value;

    class Entity
    {
    public:
        Entity();
        ~Entity() = default;

        template <IsComponent T>
        T& AddComponent()
        {
            auto it = m_components.find(std::type_index(typeid(T)));
            if (it != m_components.end())
            {
                return *static_cast<T*>(it->second.get());
            }

            UniquePtr<T> component = MakeUnique<T>();
            T& ref = *component;   
            m_components[TypeID<T>()] = std::move(component);
            return ref;
        }

        template <IsComponent T>
        void RemoveComponent()
        {
            m_components.erase(TypeID<T>());
        }

        template <IsComponent T>
        T* GetComponent()
        {
            auto it = m_components.find(TypeID<T>());
            if (it != m_components.end())
            {
                return nullptr;
            }
            return static_cast<T*>(it->second.get());
        }

        template <IsComponent T>
        bool HasComponent()
        {
            auto it = m_components.find(TypeID<T>());
            return it != m_components.end();
        }
        
    protected:
        TypeMap<UniquePtr<Component>> m_components;
    };
}