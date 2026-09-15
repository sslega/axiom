#include "DeleteEntityCommand.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace axiom
{
    DeleteEntityCommand::DeleteEntityCommand(Scene &scene, Entity& target)
    : m_scene(scene)
    , m_target(target)
    {
    }

    void DeleteEntityCommand::Execute()
    {
        m_entity = m_scene.RemoveEntity(m_target);
    }
    
    void DeleteEntityCommand::Undo()
    {
        m_scene.AddEntity(m_entity);
    }
}