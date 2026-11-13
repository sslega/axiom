#pragma once

#include "Core/Types.h"
#include "Scene/Component.h"
#include "Core/Object.h"

namespace axiom
{

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value;

    /// A thing in the world: a UUID, a name and at most one component per type.
    ///
    /// @code{.cpp}
    /// auto& transform = entity.CreateComponent<TransformComponent>();  // returns the existing one if present
    /// if (auto* mesh = entity.GetComponent<MeshComponent>()) { }       // null if missing
    /// UniquePtr<Component> removed = entity.RemoveComponent(typeid(MeshComponent)); // ownership comes back
    /// entity.AddComponent(std::move(removed));
    /// @endcode
    ///
    /// A component's OnRegister() runs when it joins the entity. Its OnInitialize() runs once the entity is in a
    /// scene with its components attached, so siblings can be looked up there whatever order they were added in.
    ///
    /// A component added to an entity already in a scene initializes straight away, so a sibling can still be
    /// missing. Other entities may not exist yet during OnInitialize().
    class Entity : public Object
    {
        friend class EngineReflection;
    public:
        ~Entity();

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
            AddComponent(std::move(component));
            return *componentPtr;
        }

        void AddComponent(UniquePtr<Component> component);
        UniquePtr<Component> RemoveComponent(std::type_index type);

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
        bool HasComponent() const
        {
            return HasComponent(TypeID<T>());
        }

        bool HasComponent(std::type_index type) const;

        Vector<Component*> GetComponents() const;
        void GetComponents(Vector<Component*>& outComponents) const;
        const String& GetName() const { return m_name; }
        void SetName(const String& name) { m_name = name; }

        Vector<Component*> FindComponentsRequiring(std::type_index componentType) const;
        Vector<std::type_index> FindMissingRequirements(const Component& component) const;

        void Deserialize(Archive& ar) override;
        void Serialize(Archive& ar) override;

        
    protected:
        TypeMap<UniquePtr<Component>> m_components;

        virtual void OnRegister()   {}
        virtual void OnUnregister() {}
        virtual void OnInitialize();
        virtual void OnShutdown();
        virtual void OnUpdate(float deltaTime);
        virtual void OnBeginFrame();
        virtual void OnRender();
        virtual void OnEndFrame();

        String m_name;

    private:
        friend class Scene;
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { m_live = true; OnInitialize(); }
        void Shutdown()              { OnShutdown(); m_live = false; }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }

        bool m_live = false; // in a scene, with its components initialized
    };
}