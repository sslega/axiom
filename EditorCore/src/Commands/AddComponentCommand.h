#pragma once
#include "Commands/ICommand.h"
#include "Core/Types.h"

namespace axiom
{
    class Entity;
    class Component;
    struct TypeDescriptor;

    /// Adds a component to an entity, together with every component it requires that the entity lacks,
    /// recursively, as one undo step. Requirements are added before the components that need them.
    ///
    /// @code{.cpp}
    /// m_undoStack.Execute(MakeUnique<AddComponentCommand>(entity, *Reflection::Get().Find(typeid(CameraController))));
    /// @endcode
    ///
    /// What's missing is worked out when the command is built, so build it right before executing it.
    /// Undo takes off everything it added and keeps the objects, so Redo restores the same ones with their values.
    /// A required type that isn't reflected is an assert.
    class AddComponentCommand : public ICommand
    {
    public:
        AddComponentCommand(Entity& entity, const TypeDescriptor& typeDescriptor);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const override { return "AddComponentCommand"; }
    private:
        Entity& m_entity;
        Vector<UniquePtr<Component>> m_components;
        Vector<std::type_index> m_componentTypes;
        void Collect(const TypeDescriptor& componentType);
    };

}