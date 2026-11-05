#pragma once
#include "Core/Types.h"
#include <imgui.h>

namespace axiom
{
    /// Base class of a dockable editor panel. EditorUI creates and owns the panels.
    ///
    /// Derive from it and implement OnRender() with ImGui. The constructor takes only what the panel needs.
    /// Scene changes go through UndoStack, and shared state stays in EditorUI.
    ///
    /// @code{.cpp}
    /// class MyPanel : public EditorPanel
    /// {
    /// public:
    ///     MyPanel(Selection& selection) : EditorPanel("My Panel"), m_selection(selection) {}
    ///     void OnRender() override { ImGui::Begin(m_name.c_str()); ImGui::End(); }
    /// private:
    ///     Selection& m_selection;
    /// };
    /// @endcode
    class EditorPanel
    {
    public:
        EditorPanel(const String& name) : m_name(name){};
        virtual ~EditorPanel() = default;

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() = 0;
        virtual const String& GetName() { return m_name; };
    protected:
        String m_name = "EditorPanel";
    };
}