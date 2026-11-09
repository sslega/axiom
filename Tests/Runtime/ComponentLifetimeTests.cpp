#include "RuntimeTest.h"

#include "Commands/AddComponentCommand.h"
#include "Commands/PropertyEditCommand.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/UndoStack.h"
#include "Reflection/Reflection.h"

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "a mesh component removed during a frame comes back as the same object on undo, and goes again on redo")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            Component* const original = entity->GetComponent<MeshComponent>();

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<RemoveComponentCommand>(*entity, typeid(MeshComponent))); });
            CHECK_FALSE(entity->HasComponent<MeshComponent>());
            RunFrames(3);

            DuringNextFrame([&] { Undo(); });
            CHECK(entity->GetComponent<MeshComponent>() == original);
            RunFrames(1);

            DuringNextFrame([&] { Redo(); });
            CHECK_FALSE(entity->HasComponent<MeshComponent>());
            RunFrames(1);

            DuringNextFrame([&] { Undo(); });
            CHECK(entity->GetComponent<MeshComponent>() == original);
            RunFrames(1);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "adding a component during a frame undoes and redoes with frames between")
        {
            Entity* entity = CreateSelectedEntity();
            entity->CreateComponent<TransformComponent>();
            const TypeDescriptor* meshType = Reflection::Get().Find(typeid(MeshComponent));
            REQUIRE(meshType);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<AddComponentCommand>(*entity, *meshType)); });
            CHECK(entity->HasComponent<MeshComponent>());
            RunFrames(2);

            DuringNextFrame([&] { Undo(); });
            CHECK_FALSE(entity->HasComponent<MeshComponent>());
            RunFrames(2);

            DuringNextFrame([&] { Redo(); });
            CHECK(entity->HasComponent<MeshComponent>());
            RunFrames(2);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "an earlier property edit still undoes after the component is removed and restored")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();
            const String pathBefore = mesh->mesh.GetPath();
            const FieldDescriptor* meshField = Reflection::Get().Find(typeid(MeshComponent))->FindField("mesh");
            REQUIRE(meshField);

            GetUndoStack().Execute(MakeUnique<PropertyEditCommand>(mesh, meshField, meshField->get(mesh), ResourceValue::Resolve("", GetResources())));
            CHECK(mesh->mesh.GetPath().empty());
            RunFrames(1);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<RemoveComponentCommand>(*entity, typeid(MeshComponent))); });
            RunFrames(1);

            DuringNextFrame([&] { Undo(); });
            CHECK(entity->GetComponent<MeshComponent>() == mesh);
            RunFrames(1);

            DuringNextFrame([&] { Undo(); });
            CHECK(mesh->mesh.GetPath() == pathBefore);
            RunFrames(1);
        }
    }
}
