#include "Selection.h"
#include "Commands/CreateEntityCommand.h"
#include "Commands/DeleteEntityCommand.h"
#include "Commands/UndoStack.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <doctest/doctest.h>

namespace axiom::tests
{
    TEST_SUITE("logic")
    {
        namespace
        {
            // These compare pointer values only: the entity may be freed at the point of a check.
            struct SelectionFixture
            {
                Scene scene{ "test" };
                Selection selection{ scene };
                UndoStack stack;

                Entity* CreateAndSelect()
                {
                    UniquePtr<CreateEntityCommand> command = MakeUnique<CreateEntityCommand>(scene);
                    Entity* entity = command->GetEntity();
                    stack.Execute(std::move(command));
                    selection.Set(entity);
                    return entity;
                }
            };
        }

        TEST_CASE_FIXTURE(SelectionFixture, "undoing a selected entity's creation leaves nothing selected")
        {
            CreateAndSelect();

            stack.Undo();

            CHECK(selection.Get() == nullptr);
        }

        TEST_CASE_FIXTURE(SelectionFixture, "removing a selected entity from the scene leaves nothing selected")
        {
            Entity* entity = CreateAndSelect();

            stack.Execute(MakeUnique<DeleteEntityCommand>(scene, *entity));

            CHECK(selection.Get() == nullptr);
        }

        TEST_CASE_FIXTURE(SelectionFixture, "redo that brings a selected entity back selects it again")
        {
            Entity* entity = CreateAndSelect();

            stack.Undo();
            CHECK(selection.Get() == nullptr);

            stack.Redo();
            CHECK(selection.Get() == entity);
        }

        TEST_CASE_FIXTURE(SelectionFixture, "undoing a selected entity's deletion selects it again")
        {
            Entity* entity = CreateAndSelect();

            stack.Execute(MakeUnique<DeleteEntityCommand>(scene, *entity));
            CHECK(selection.Get() == nullptr);

            stack.Undo();
            CHECK(selection.Get() == entity);
        }

        TEST_CASE_FIXTURE(SelectionFixture, "a new command after undo destroys the entity, and the selection stays empty")
        {
            CreateAndSelect();
            stack.Undo();

            // Drops the redo history, which was the last owner of the entity.
            stack.Execute(MakeUnique<CreateEntityCommand>(scene));

            CHECK(selection.Get() == nullptr);
        }
    }
}
