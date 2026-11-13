#pragma once

#include "Core/Object.h"

namespace axiom
{
    class Entity;
    class Archive;

    /// Base class of data and behaviour attached to an Entity.
    ///
    /// To add a component type:
    /// 1. Derive from it, next to the system that consumes it, and override the `On*` hooks you need.
    /// 2. Register the type and its fields in EngineReflection.cpp. That gives you loading, saving, the Inspector and Add Component.
    ///
    /// OnInitialize() runs once the entity is complete, so siblings can be looked up there. Entity explains the limits.
    ///
    /// A component that needs a sibling declares it by overriding GetRequiredComponents(). Add Component and
    /// scene loading then add the sibling first, and the Inspector refuses to remove it, so the component can
    /// trust it's there instead of checking. Only the editor commands and the loader enforce this; code calling
    /// Entity::AddComponent or RemoveComponent directly is unchecked.
    ///
    /// @code{.cpp}
    /// class SpinComponent : public Component
    /// {
    /// public:
    ///     float speed = 1.0f;
    ///     const Vector<std::type_index>& GetRequiredComponents() const override
    ///     {
    ///         static const Vector<std::type_index> required = { TypeID<TransformComponent>() };
    ///         return required;
    ///     }
    /// protected:
    ///     void OnUpdate(float deltaTime) override
    ///     {
    ///         TransformComponent* transform = GetEntity().GetComponent<TransformComponent>();
    ///     }
    /// };
    /// @endcode
    class Component : public Object
    {
    public:
        virtual ~Component() = default;

        Entity& GetEntity() const;
        
        const virtual Vector<std::type_index>& GetRequiredComponents() const;
        bool RequiresComponent(std::type_index componentType) const;
        
        void Deserialize(Archive& ar) override;
        void Serialize(Archive& ar) override;

    protected:
        virtual void OnRegister()   {}
        virtual void OnUnregister() {}
        virtual void OnInitialize() {}
        virtual void OnShutdown()   {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender()     {}
        virtual void OnEndFrame()   {}

    private:
        friend class Entity;
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { OnInitialize(); }
        void Shutdown()              { OnShutdown(); }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }

        Entity* m_entity = nullptr;
    };
}