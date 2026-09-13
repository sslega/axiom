#pragma once

namespace axiom
{
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
        virtual const char* GetName() const { return "Command"; }
    };
}
