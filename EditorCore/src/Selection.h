#pragma once

#include "Core/Types.h"
#include "Scene/Scene.h"

namespace axiom
{
    /// The currently selected entity. EditorUI owns it and passes it to panels.
    ///
    /// It stores the entity's ID and looks the entity up in the scene on every Get(), so it can't dangle.
    /// While the entity is out of the scene, Get() returns null; if undo or redo brings it back, it's selected again.
    ///
    /// @code{.cpp}
    /// if (Entity* entity = selection.Get()) { }
    /// selection.Set(entity);
    /// selection.Clear();
    /// @endcode
    class Selection
    {
    public:
        explicit Selection(Scene& scene) : m_scene(scene) {}

        Entity* Get() const { return m_scene.FindEntity(m_id); }
        void Set(Entity* inEntity) { m_id = inEntity ? inEntity->GetID() : String(); }
        void Clear() { m_id.clear(); }
        bool IsSelected(const Entity* inEntity) const { return inEntity == Get(); }
    
    private:
        Scene& m_scene;
        String m_id;
    };
}