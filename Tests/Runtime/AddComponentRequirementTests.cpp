#include "RuntimeTest.h"
#include "TestComponents.h"

#include "Commands/AddComponentCommand.h"
#include "Commands/UndoStack.h"
#include "Reflection/Reflection.h"
#include "Scene/Entity.h"

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        TEST_CASE_FIXTURE(RuntimeTest, "adding a component adds its missing requirement chain as one undo step")
        {
            Entity* entity = CreateSelectedEntity();
            const TypeDescriptor* topType = Reflection::Get().Find(typeid(ReqTop));
            REQUIRE(topType);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<AddComponentCommand>(*entity, *topType)); });
            REQUIRE(entity->HasComponent<ReqTop>());
            REQUIRE(entity->HasComponent<ReqMiddle>());
            REQUIRE(entity->HasComponent<ReqLeaf>());
            Component* const top = entity->GetComponent<ReqTop>();
            Component* const middle = entity->GetComponent<ReqMiddle>();
            Component* const leaf = entity->GetComponent<ReqLeaf>();
            RunFrames(1);

            // One undo takes all three off.
            DuringNextFrame([&] { Undo(); });
            CHECK_FALSE(entity->HasComponent<ReqTop>());
            CHECK_FALSE(entity->HasComponent<ReqMiddle>());
            CHECK_FALSE(entity->HasComponent<ReqLeaf>());
            RunFrames(1);

            // Redo restores the same objects, not new ones.
            DuringNextFrame([&] { Redo(); });
            CHECK(entity->GetComponent<ReqTop>() == top);
            CHECK(entity->GetComponent<ReqMiddle>() == middle);
            CHECK(entity->GetComponent<ReqLeaf>() == leaf);
            RunFrames(1);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "adding a component keeps a requirement already present and undo leaves it")
        {
            Entity* entity = CreateSelectedEntity();
            ReqLeaf& leaf = entity->CreateComponent<ReqLeaf>();
            ReqMiddle& middle = entity->CreateComponent<ReqMiddle>();
            const TypeDescriptor* topType = Reflection::Get().Find(typeid(ReqTop));
            REQUIRE(topType);

            DuringNextFrame([&] { GetUndoStack().Execute(MakeUnique<AddComponentCommand>(*entity, *topType)); });
            CHECK(entity->HasComponent<ReqTop>());
            CHECK(entity->GetComponent<ReqMiddle>() == &middle);
            CHECK(entity->GetComponent<ReqLeaf>() == &leaf);
            RunFrames(1);

            // Undo removes only what the add created.
            DuringNextFrame([&] { Undo(); });
            CHECK_FALSE(entity->HasComponent<ReqTop>());
            CHECK(entity->GetComponent<ReqMiddle>() == &middle);
            CHECK(entity->GetComponent<ReqLeaf>() == &leaf);
            RunFrames(1);
        }
    }
}
