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
        using ID = uint32_t;
        Entity();
        Entity(String name);
        ~Entity() = default;

        const ID id;

        template <IsComponent T, typename... Args>
        T& CreateComponent(Args&&... args)
        {
            auto it = m_components.find(std::type_index(typeid(T)));
            if (it != m_components.end())
            {
                return static_cast<T&>(*it->second.get());
            }

            UniquePtr<T> component = MakeUnique<T>(std::forward<Args>(args)...);
            T* componentPtr = component.get();
            componentPtr->m_entity = this;
            m_components[TypeID<T>()] = std::move(component);
            componentPtr->Register();
            componentPtr->Initialize();
            return *componentPtr;
        }

        template <IsComponent T>
        void DestroyComponent()
        {
            auto it = m_components.find(TypeID<T>());
            if (it != m_components.end())
            {
                it->second->Shutdown();
                it->second->Unregister();
                m_components.erase(it);
            }
        }

        template <IsComponent T>
        T* GetComponent()
        {
            auto it = m_components.find(TypeID<T>());
            if (it == m_components.end())
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

        virtual void OnRegister()   {}
        virtual void OnUnregister();
        virtual void OnInitialize() {}
        virtual void OnShutdown();
        virtual void OnUpdate(float deltaTime);
        virtual void OnBeginFrame();
        virtual void OnRender();
        virtual void OnEndFrame();

    private:
        friend class Scene;
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { OnInitialize(); }
        void Shutdown()              { OnShutdown(); }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }

        inline static ID s_nextId = 0;
        String m_name;
    };
}