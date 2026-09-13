#pragma once

#include "Core/Types.h"
#include "UI/Commands/ICommand.h"

namespace axiom
{
    class UndoStack
    {
    public:
        void Execute(UniquePtr<ICommand> command);
        inline bool CanUndo(){ return m_head > 0; }
        inline bool CanRedo(){ return m_head < m_commands.size(); }
        void Undo();
        void Redo();
    private:
        size_t m_head = 0;
        Vector<UniquePtr<ICommand>> m_commands;
    };
}