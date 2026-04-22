#pragma once

namespace axiom
{
    class Entity;

    class Component
    {
    public:
        virtual ~Component() = default;

        Entity& GetEntity() const;

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