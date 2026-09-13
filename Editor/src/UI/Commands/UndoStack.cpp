#include "UndoStack.h"
#include "UI/Commands/ICommand.h"

namespace axiom
{
    void UndoStack::Execute(UniquePtr<ICommand> command)
    {
        m_commands.resize(m_head);
        m_commands.push_back(std::move(command));
        m_commands[m_head]->Execute();
        m_head++;
    }

    void UndoStack::Undo()
    {
        if(CanUndo())
        {
            m_head--;
            m_commands[m_head]->Undo();
        }
    }
    
    void UndoStack::Redo()
    {
        if(CanRedo())
        {
            m_commands[m_head]->Execute();
            m_head++;
        }
    }
}