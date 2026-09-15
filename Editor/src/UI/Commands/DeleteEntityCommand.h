#pragma once
#include "UI/Commands/ICommand.h"
#include "Core/Types.h"

namespace axiom
{
    class Scene;
    class Entity;

    class DeleteEntityCommand : public ICommand
    {
    public:
        DeleteEntityCommand(Scene& scene, Entity& target);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const override { return "DeleteEntityCommand"; }
    private:
        Scene& m_scene;
        Entity& m_target;
        SharedPtr<Entity> m_entity = nullptr;       
    };
}