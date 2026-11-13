#include "Scene/Component.h"
#include "Scene/Entity.h"
#include <doctest/doctest.h>

#include <algorithm>

namespace axiom::tests
{
    TEST_SUITE("logic")
    {
        namespace
        {
            class FakeA  : public Component
            {
            };

            class FakeB : public Component
            {
            public:
                const Vector<std::type_index>& GetRequiredComponents() const override
                {
                    static const Vector<std::type_index> required = { TypeID<FakeA>() };
                    return required;
                }
            };
            
            class FakeC : public Component
            {
            public:
                const Vector<std::type_index>& GetRequiredComponents() const override
                {
                    static const Vector<std::type_index> required = { TypeID<FakeA>() };
                    return required;
                }
            };
        }

        TEST_CASE("a component that requires a type is found as requiring it")
        {
            Entity entity;
            entity.CreateComponent<FakeA>();
            Component& fakeB = entity.CreateComponent<FakeB>();

            Vector<Component*> requiring = entity.FindComponentsRequiring(TypeID<FakeA>());

            REQUIRE(requiring.size() == 1);
            CHECK(requiring[0] == &fakeB);
        }

        TEST_CASE("nothing is found requiring a type no component needs")
        {
            Entity entity;
            entity.CreateComponent<FakeA>();
            
            Vector<Component*> requiring = entity.FindComponentsRequiring(TypeID<FakeB>());

            CHECK(requiring.size() == 0);
        }

        TEST_CASE("every component that requires a type is found as requiring it")
        {
            Entity entity;
            Component& fakeB = entity.CreateComponent<FakeB>();
            Component& fakeC = entity.CreateComponent<FakeC>();
            
            Vector<Component*> requiring = entity.FindComponentsRequiring(TypeID<FakeA>());
            
            // The entity's components are unordered, so check membership, not position.
            REQUIRE(requiring.size() == 2);
            CHECK(std::ranges::find(requiring, &fakeB) != requiring.end());
            CHECK(std::ranges::find(requiring, &fakeC) != requiring.end());
        }

        TEST_CASE("a component is missing a required type the entity lacks")
        {
            Entity entity;
            Component& fakeB = entity.CreateComponent<FakeB>();

            Vector<std::type_index> missingRequirements = entity.FindMissingRequirements(fakeB);
            REQUIRE(missingRequirements.size() == 1);
            CHECK(missingRequirements[0] == TypeID<FakeA>());
        }

        TEST_CASE("a component is missing nothing once its requirement is present")
        {
            Entity entity;
            entity.CreateComponent<FakeA>();
            Component& fakeB = entity.CreateComponent<FakeB>();

            Vector<std::type_index> missingRequirements = entity.FindMissingRequirements(fakeB);
            CHECK(missingRequirements.size() == 0);
        }

        TEST_CASE("a component with no requirements is missing nothing")
        {
            Entity entity;
            Component& component = entity.CreateComponent<FakeA>();

            Vector<std::type_index> missingRequirements = entity.FindMissingRequirements(component);
            CHECK(missingRequirements.size() == 0);
        }
    }
}