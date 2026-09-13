#pragma once

#include "AxiomEngine.h"
#include "UI/EditorPanel.h"
#include "UI/Selection.h"
#include "UI/Commands/UndoStack.h"

using namespace axiom;

namespace axiom
{
    
    class EditorUI
    {
    public:
        EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem);

        inline Scene& GetActiveScene() { return m_worldSubsystem.GetActiveScene(); };
        
        void OnInitialize();
        void OnUpdate(float dt);
        void OnRender();
    
    private:
        RenderSubsystem& m_renderSubsystem;
        WorldSubsystem& m_worldSubsystem;

        Selection m_selection;
        UndoStack m_undoStack;

        Vector<UniquePtr<EditorPanel>> m_panels;
        
        void AddViewportPanel(const String& name);
        void AddOutlinerPanel();
        void AddInspectorPanel();
    };
}