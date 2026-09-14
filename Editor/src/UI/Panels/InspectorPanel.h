#pragma once
#include "UI/EditorPanel.h"
#include "Reflection/Reflection.h"
#include "UI/Commands/UndoStack.h"
#include "UI/Commands/PropertyEditCommand.h"

namespace axiom
{
    class Selection;

    class InspectorPanel : public EditorPanel
    {
    public:
        InspectorPanel(Selection& selection, UndoStack& undoStack): 
        EditorPanel("Inspector"), m_selection(selection), m_undoStack(undoStack) {}

        template<typename T>
        void CommitField(Component* component, const FieldDescriptor& field, FieldValue oldValue, bool activated, bool changed, bool deactivatedAfterEdit, const T& local)
        {
            if (changed)
            {
                field.set(component, FieldValue(local));
            }
            if (activated)
            {
                m_pendingOldValue = oldValue;
            }
            if (deactivatedAfterEdit)
            {
                m_undoStack.Execute(MakeUnique<PropertyEditCommand>(component, &field, m_pendingOldValue, FieldValue(local)));
            }
        }


        virtual void OnRender() override;
    private:
        Selection& m_selection;
        UndoStack& m_undoStack;
        FieldValue m_pendingOldValue;
    };
}