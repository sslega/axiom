#pragma once

#include "Core/Types.h"
#include "Commands/ICommand.h"

namespace axiom
{
    /// Runs commands so they can be undone. Panels change the scene only through this.
    ///
    /// Execute() runs the command and drops any redo history.
    ///
    /// @code{.cpp}
    /// m_undoStack.Execute(MakeUnique<DeleteEntityCommand>(scene, *entity));
    /// @endcode
    class UndoStack
    {
    public:
        void Execute(UniquePtr<ICommand> command);
        inline bool CanUndo(){ return m_head > 0; }
        inline bool CanRedo(){ return m_head < m_commands.size(); }
        void Undo();
        void Redo();
        void Clear();
    private:
        size_t m_head = 0;
        Vector<UniquePtr<ICommand>> m_commands;
    };
}