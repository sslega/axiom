#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <doctest/doctest.h>

namespace axiom::tests
{
    TEST_SUITE("logic")
    {
        TEST_CASE("a scene finds an entity by its ID")
        {
            Scene scene("test");
            SharedPtr<Entity> entity = scene.CreateEntity();

            CHECK(scene.FindEntity(entity->GetID()) == entity.get());
        }

        TEST_CASE("looking up an unknown ID gives null")
        {
            Scene scene("test");
            scene.CreateEntity();

            CHECK(scene.FindEntity("not-an-id") == nullptr);
            CHECK(scene.FindEntity("") == nullptr);
        }

        TEST_CASE("an entity removed from the scene can no longer be found by ID")
        {
            Scene scene("test");
            SharedPtr<Entity> entity = scene.CreateEntity();

            scene.RemoveEntity(*entity);

            CHECK(scene.FindEntity(entity->GetID()) == nullptr);
        }
    }
}
