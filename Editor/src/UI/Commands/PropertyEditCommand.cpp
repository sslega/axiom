#include "PropertyEditCommand.h"
#include "Core/Object.h"
#include "Reflection/ReflectionField.h"

namespace axiom
{
    PropertyEditCommand::PropertyEditCommand(Object *object, const FieldDescriptor* fieldDescriptor, FieldValue oldValue, FieldValue newValue)
    : m_target(object)
    , m_fieldDescriptor(fieldDescriptor)
    , m_oldValue(oldValue)
    , m_newValue(newValue)
    {
    }

    void PropertyEditCommand::Execute()
    {
        m_fieldDescriptor->set(m_target, m_newValue);
    }

    void PropertyEditCommand::Undo()
    {
        m_fieldDescriptor->set(m_target, m_oldValue);
    }
}