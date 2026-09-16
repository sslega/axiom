#pragma once

#include "UI/Commands/ICommand.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    class Object;
    class FieldDescriptor;

    class PropertyEditCommand : public ICommand
    {
    public:
        PropertyEditCommand(Object* target, const FieldDescriptor* fieldDescriptor, FieldValue oldValue, FieldValue newValue);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const { return "PropertyEditCommand"; }
    private:
        Object* m_target;
        const FieldDescriptor* m_fieldDescriptor;
        FieldValue m_oldValue;
        FieldValue m_newValue;
    };
}