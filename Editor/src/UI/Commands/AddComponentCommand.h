#pragma once
#include "UI/Commands/ICommand.h"
#include "Core/Types.h"

namespace axiom
{
    class Entity;
    class Component;
    struct TypeDescriptor;

    class AddComponentCommand : public ICommand
    {
    public:
        AddComponentCommand(Entity& entity, const TypeDescriptor& typeDescriptor);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const override { return "AddComponentCommand"; }
    private:
        Entity& m_entity;
        const TypeDescriptor& m_typeDescriptor;
        UniquePtr<Component> m_component;
    };

}