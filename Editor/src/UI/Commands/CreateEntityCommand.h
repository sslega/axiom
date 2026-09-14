#pragma once
#include "Core/Types.h"
#include "UI/Commands/ICommand.h"

namespace axiom
{
    class Scene;
    class Entity;

    class CreateEntityCommand : public ICommand
    {
    public:
        CreateEntityCommand(Scene& scene);
        virtual void Execute() override;
        virtual void Undo() override;
        virtual const char* GetName() const override { return "CreateEntityCommand"; }

        Entity* GetEntity() const { return m_entity.get(); }
    private:
        Scene& m_scene;
        SharedPtr<Entity> m_entity;
    };

}