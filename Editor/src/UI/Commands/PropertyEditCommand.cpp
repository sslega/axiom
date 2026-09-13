#include "PropertyEditCommand.h"
#include "Scene/Component.h"
#include "Reflection/ReflectionField.h"

namespace axiom
{
    PropertyEditCommand::PropertyEditCommand(Component *component, const FieldDescriptor* fieldDescriptor, FieldValue oldValue, FieldValue newValue)
    : m_component(component)
    , m_fieldDescriptor(fieldDescriptor)
    , m_oldValue(oldValue)
    , m_newValue(newValue)
    {
    }

    void PropertyEditCommand::Execute()
    {
        m_fieldDescriptor->set(m_component, m_newValue);
    }

    void PropertyEditCommand::Undo()
    {
        m_fieldDescriptor->set(m_component, m_oldValue);
    }
}