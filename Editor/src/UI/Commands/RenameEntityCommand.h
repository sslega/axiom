#pragma once
#include "Core/Types.h"
#include "UI/Commands/ICommand.h"

namespace axiom
{
    class Entity;

    class RenameEntityCommand : public ICommand
    {
    public:
        RenameEntityCommand(Entity& entity, const String& oldName, const String& newName);
        void Execute() override;
        void Undo() override;
        const char* GetName() const override { return "RenameEntityCommand"; }
    private:
        Entity& m_entity;
        String m_oldName;
        String m_newName;
    };
}