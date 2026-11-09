#pragma once

#include "Commands/ICommand.h"
#include "Core/Types.h"

namespace axiom
{
    class Entity;
    class Component;

    /// Takes a component off an entity. Undo gives back the same object, with every value it had,
    /// so earlier undo steps that point at it keep working.
    ///
    /// @code{.cpp}
    /// m_undoStack.Execute(MakeUnique<RemoveComponentCommand>(entity, typeid(MeshComponent)));
    /// @endcode
    ///
    /// Don't run it while walking the entity's components. Anything that caches a pointer to the
    /// removed component, such as CameraController with its transform, is left dangling.
    class RemoveComponentCommand : public ICommand
    {
    public:
        RemoveComponentCommand(Entity& entity, std::type_index componentType);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const override { return "RemoveComponentCommand"; }
    private:
        Entity& m_entity;
        std::type_index m_componentType;
        UniquePtr<Component> m_component;
    };
}
