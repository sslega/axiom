#include "RenameEntityCommand.h"
#include "Scene/Entity.h"

namespace axiom
{
    RenameEntityCommand::RenameEntityCommand(Entity& entity, const String& oldName, const String& newName)
    : m_entity(entity)
    , m_oldName(oldName)
    , m_newName(newName)
    {
    }

    void RenameEntityCommand::Execute()
    {
        m_entity.SetName(m_newName);
    }
    
    void RenameEntityCommand::Undo()
    {
        m_entity.SetName(m_oldName);
    }
}