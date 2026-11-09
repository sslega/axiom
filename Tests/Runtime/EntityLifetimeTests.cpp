#include "RuntimeTest.h"

#include "Commands/CreateEntityCommand.h"
#include "Commands/DeleteEntityCommand.h"
#include "Commands/PropertyEditCommand.h"
#include "Commands/UndoStack.h"
#include "Reflection/Reflection.h"
#include "Selection.h"

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "creating an entity undoes and redoes with frames between")
        {
            Entity* entity = CreateSelectedEntity();
            entity->CreateComponent<TransformComponent>();
            const String id = entity->GetID();
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == entity);

            Undo();
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == nullptr);

            Redo();
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == entity);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "deleting an entity undoes and redoes with frames between")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            const String id = entity->GetID();

            GetUndoStack().Execute(MakeUnique<DeleteEntityCommand>(GetScene(), *entity));
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == nullptr);

            Undo();
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == entity);

            Redo();
            RunFrames(2);
            CHECK(GetScene().FindEntity(id) == nullptr);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "deleting the selected mesh entity during a frame deselects it, and undo reselects it")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<DeleteEntityCommand>(GetScene(), *entity)); });
            CHECK(GetSelection().Get() == nullptr);
            RunFrames(2);

            DuringNextFrame([&] { Undo(); });
            CHECK(GetSelection().Get() == entity);
            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "undoing a selected entity's creation deselects it, and frames keep running")
        {
            CreateSelectedEntity()->CreateComponent<TransformComponent>();
            RunFrames(1);

            Undo();
            RunFrames(2);

            CHECK(GetSelection().Get() == nullptr);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "redoing a selected entity's creation selects it again, and frames keep running")
        {
            Entity* entity = CreateSelectedEntity();
            entity->CreateComponent<TransformComponent>();

            Undo();
            RunFrames(1);
            Redo();
            RunFrames(2);

            CHECK(GetSelection().Get() == entity);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "a new field edit after undoing a selected entity's creation leaves the selection empty")
        {
            SharedPtr<Entity> other = GetScene().CreateEntity();
            const FieldDescriptor* nameField = Reflection::Get().Find(typeid(Entity))->FindField("name");
            REQUIRE(nameField);

            CreateSelectedEntity();
            Undo();
            RunFrames(1);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<PropertyEditCommand>(other.get(), nameField, nameField->get(other.get()), FieldValue(String("Renamed")))); });
            RunFrames(2);

            CHECK(GetSelection().Get() == nullptr);
            CHECK(other->GetName() == "Renamed");
        }

        TEST_CASE_FIXTURE(RuntimeTest, "a new command after undo drops the redo history that owned an entity, and frames keep running")
        {
            CreateSelectedEntity();
            Undo();
            RunFrames(1);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<CreateEntityCommand>(GetScene())); });
            CHECK_FALSE(GetUndoStack().CanRedo());
            RunFrames(2);

            Undo();
            RunFrames(1);
            CHECK(GetScene().GetEntities().empty());
        }
    }
}
