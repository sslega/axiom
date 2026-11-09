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
    /// @code{.cpp}
    /// class SpinComponent : public Component
    /// {
    /// public:
    ///     float speed = 1.0f;
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