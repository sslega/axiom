#pragma once

namespace axiom
{
    class Entity;

    /// The currently selected entity. EditorUI owns it and passes it to panels.
    ///
    /// It holds a plain pointer and doesn't own the entity, so whoever removes the selected entity must clear it.
    ///
    /// @code{.cpp}
    /// if (Entity* entity = selection.Get()) { }
    /// selection.Set(entity);
    /// selection.Clear();
    /// @endcode
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