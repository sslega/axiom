#include "AddComponentCommand.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"
#include "Reflection/Reflection.h"
namespace axiom
{
    AddComponentCommand::AddComponentCommand(Entity& entity, const TypeDescriptor& typeDescriptor)
    : m_entity(entity)
    , m_typeDescriptor(typeDescriptor)
    {
        m_component = m_typeDescriptor.factory();
    }
    
    void AddComponentCommand::Execute()
    {
        m_entity.AddComponent(std::move(m_component));
    }
    
    void AddComponentCommand::Undo()
    {
        m_component = m_entity.RemoveComponent(m_typeDescriptor.type);
    }
}