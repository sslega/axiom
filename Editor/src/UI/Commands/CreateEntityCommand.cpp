#include "CreateEntityCommand.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace axiom
{
    CreateEntityCommand::CreateEntityCommand(Scene &scene)
    : m_scene(scene)
    , m_entity(MakeShared<Entity>())
    {
    }
    
    void CreateEntityCommand::Execute()
    {
        m_scene.AddEntity(m_entity);
    }
    
    void CreateEntityCommand::Undo()
    {
        m_scene.RemoveEntity(*m_entity);
    }
}