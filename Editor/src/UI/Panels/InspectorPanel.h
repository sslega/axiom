#pragma once
#include "UI/EditorPanel.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    class Selection;
    class UndoStack;

    class InspectorPanel : public EditorPanel
    {
    public:
        InspectorPanel(Selection& selection, UndoStack& undoStack): 
        EditorPanel("Inspector"), m_selection(selection), m_undoStack(undoStack) {}

        virtual void OnRender() override;
    private:
        Selection& m_selection;
        UndoStack& m_undoStack;
        FieldValue m_pendingOldValue;
    };
}