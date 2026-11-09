#pragma once

#include "AxiomEngine.h"
#include "UI/EditorPanel.h"
#include "Selection.h"
#include "UI/GizmoState.h"
#include "Commands/UndoStack.h"

using namespace axiom;

namespace axiom
{
    class ViewportPanel;
    
    /// Root of the editor UI: the dock space, main menu bar, keyboard shortcuts and all panels.
    /// Owns the Selection, UndoStack and GizmoState, and passes them to the panels that need them.
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
        EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem, FileSubsystem& fileSubsystem, ResourceSubsystem& resourceSubsystem);

        inline Scene& GetActiveScene() { return m_worldSubsystem.GetActiveScene(); };
        inline Selection& GetSelection() { return m_selection; }
        inline UndoStack& GetUndoStack() { return m_undoStack; }

        void OnInitialize();
        void OnUpdate(float dt);
        void OnRender();

        void NewScene();

        /// Replaces the active scene with the file. If it can't be loaded, the scene and undo history stay as they were.
        bool LoadScene(const String& virtualPath);
    
    private:
        RenderSubsystem& m_renderSubsystem;
        WorldSubsystem& m_worldSubsystem;
        FileSubsystem& m_fileSubsystem;
        ResourceSubsystem& m_resourceSubsystem;

        Selection m_selection;
        UndoStack m_undoStack;
        GizmoState m_gizmoState;

        Vector<UniquePtr<EditorPanel>> m_panels;
        
        void OpenScene();
        void SaveScene();
        void OnSceneReplaced();

        ViewportPanel* AddViewportPanel(const String& name);
        void AddOutlinerPanel();
        void AddInspectorPanel();
    };
}