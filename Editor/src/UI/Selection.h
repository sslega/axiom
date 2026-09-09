#pragma once

namespace axiom
{
    class Entity;

    class Selection
    {
    public:
        Entity* Get() const { return m_entity; }
        void Set(Entity* inEntity) { m_entity = inEntity; }
        void Clear() { m_entity = nullptr; }
        bool IsSelected(const Entity* inEntity) const { return inEntity == m_entity; }
    
    private:
        Entity* m_entity = nullptr;
    };
}