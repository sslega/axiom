#pragma once

namespace axiom
{
    class Entity;
    
    class Component
    {
    public:
        virtual ~Component() = default;

        Entity* GetEntity() const;
    
    protected:
        virtual void OnRegister() {}
        virtual void OnInitialize() {}
        virtual void OnShutdown() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender() {}
        virtual void OnEndFrame() {}
    
    private:
        Entity* m_entity = nullptr;
        friend class Entity;
    };
}