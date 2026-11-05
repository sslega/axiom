#pragma once

#include "AxiomEngine.h"
#include "UI/EditorPanel.h"
#include "UI/Selection.h"
#include "UI/Commands/UndoStack.h"

using namespace axiom;

namespace axiom
{
    class ViewportPanel;
    
    /// Root of the editor UI: the dock space, main menu bar, keyboard shortcuts and all panels.
    /// Owns the Selection and UndoStack, and passes them to the panels that need them.
    ///
    /// - New menu item or shortcut: OnRender().
    /// - New panel: an `Add*Panel()` function called from OnInitialize().
    ///
    /// @code{.cpp}
    /// m_panels.push_back(MakeUnique<OutlinerPanel>(m_worldSubsystem, m_selection, m_undoStack));
    /// @endcode
    class EditorUI
    {
    public:
        EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem, FileSubsystem& m_fileSubsystem);

        inline Scene& GetActiveScene() { return m_worldSubsystem.GetActiveScene(); };
        
        void OnInitialize();
        void OnUpdate(float dt);
        void OnRender();
    
    private:
        RenderSubsystem& m_renderSubsystem;
        WorldSubsystem& m_worldSubsystem;
        FileSubsystem& m_fileSubsystem;

        Selection m_selection;
        UndoStack m_undoStack;

        Vector<UniquePtr<EditorPanel>> m_panels;
        
        ViewportPanel* AddViewportPanel(const String& name);
        void AddOutlinerPanel();
        void AddInspectorPanel();
    };
}