#pragma once

namespace axiom
{
    /// One undoable action, run through UndoStack.
    ///
    /// A command holds everything it needs to reverse itself: old values, or ownership of what it removed.
    /// Redo calls Execute() again, so it must be repeatable.
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
        virtual const char* GetName() const { return "Command"; }
    };
}
