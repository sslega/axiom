#pragma once

namespace axiom
{
    class Entity;
    
    class Component
    {
    public:
        virtual ~Component() = default;

        Entity* GetEntity() const;
    
    private:
        Entity* m_entity = nullptr;
        friend class Entity;
    };
}