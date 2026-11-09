#include "RemoveComponentCommand.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"

namespace axiom
{
    RemoveComponentCommand::RemoveComponentCommand(Entity& entity, std::type_index componentType)
    : m_entity(entity)
    , m_componentType(componentType)
    {
    }

    void RemoveComponentCommand::Execute()
    {
        m_component = m_entity.RemoveComponent(m_componentType);
    }

    void RemoveComponentCommand::Undo()
    {
        m_entity.AddComponent(std::move(m_component));
    }
}
