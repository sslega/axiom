#pragma once

#include "UI/Commands/ICommand.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    class Component;
    class FieldDescriptor;

    class PropertyEditCommand : public ICommand
    {
    public:
        PropertyEditCommand(Component* component, const FieldDescriptor* fieldDescriptor, FieldValue oldValue, FieldValue newValue);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const { return "PropertyEditCommand"; }
    private:
        Component* m_component;
        const FieldDescriptor* m_fieldDescriptor;
        FieldValue m_oldValue;
        FieldValue m_newValue;
    };
}