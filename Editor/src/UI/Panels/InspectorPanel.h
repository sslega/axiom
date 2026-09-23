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
        using FieldDrawFunction = void (InspectorPanel::*)(Component*, const FieldDescriptor&, const String&);

    public:
        InspectorPanel(Selection& selection, UndoStack& undoStack);

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

        void DrawFloat(Component* component, const FieldDescriptor& field, const String& label);
        void DrawBool(Component* component, const FieldDescriptor& field, const String& label);
        void DrawInt(Component* component, const FieldDescriptor& field, const String& label);
        void DrawString(Component* component, const FieldDescriptor& field, const String& label);
        void DrawVec3(Component* component, const FieldDescriptor& field, const String& label);
        void DrawColor(Component* component, const FieldDescriptor& field, const String& label);
        UnorderedMap<FieldType, FieldDrawFunction> m_fieldDrawFunction;
    };
}