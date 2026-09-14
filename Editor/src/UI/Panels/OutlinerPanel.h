#pragma once
#include "UI/EditorPanel.h"

namespace axiom
{
    class WorldSubsystem;
    class Selection;
    class UndoStack;

    class OutlinerPanel : public EditorPanel
    {
    public:
        OutlinerPanel(WorldSubsystem& world, Selection& selection, UndoStack& undoStack);

        virtual void OnRender() override;
    
    private:
        WorldSubsystem& m_world;
        Selection& m_selection;
        UndoStack& m_undoStack;
    };
}