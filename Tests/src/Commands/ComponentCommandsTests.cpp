#include "Commands/PropertyEditCommand.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/UndoStack.h"
#include "Reflection/ReflectionField.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

#include <doctest/doctest.h>

namespace axiom::tests
{
    namespace
    {
        struct FakeResource
        {
            String path;
        };

        // Hands back the same object for the same path, like ResourceSubsystem.
        class FakeResolver : public IResourceResolver
        {
        public:
            SharedPtr<void> ResolveResource(const String& virtualPath) override
            {
                SharedPtr<FakeResource>& entry = m_cache[virtualPath];
                if (!entry)
                {
                    entry = MakeShared<FakeResource>(FakeResource{ virtualPath });
                }
                return entry;
            }

        private:
            StringMap<SharedPtr<FakeResource>> m_cache;
        };

        class FakeComponent : public Component
        {
        public:
            ResourceRef<FakeResource> mesh;
            float intensity = 1.0f;
            int initializeCount = 0;
            int shutdownCount = 0;

        protected:
            void OnInitialize() override { ++initializeCount; }
            void OnShutdown() override { ++shutdownCount; }
        };

        class OtherComponent : public Component
        {
        };

        const FieldDescriptor& MeshField()
        {
            static const FieldDescriptor field = MakeField("mesh", &FakeComponent::mesh);
            return field;
        }

        const FieldDescriptor& IntensityField()
        {
            static const FieldDescriptor field = MakeField("intensity", &FakeComponent::intensity);
            return field;
        }

        constexpr const char* kCube = "engine://Meshes/cube.obj";
        constexpr const char* kSphere = "engine://Meshes/sphere.obj";

        UniquePtr<PropertyEditCommand> MakeEdit(FakeComponent& component, FakeResolver& resolver, const String& path)
        {
            return MakeUnique<PropertyEditCommand>(&component, &MeshField(), MeshField().get(&component), ResourceValue::Resolve(path, resolver));
        }

        void Point(FakeComponent& component, FakeResolver& resolver, const String& path)
        {
            MakeEdit(component, resolver, path)->Execute();
        }

        String PathOf(const FakeComponent& component)
        {
            return std::get<ResourceValue>(MeshField().get(&component)).path;
        }
    }

    TEST_CASE("a resource field names its resource type, and other fields don't")
    {
        CHECK(MeshField().resourceType == std::type_index(typeid(FakeResource)));
        CHECK(IntensityField().resourceType == std::type_index(typeid(void)));
    }

    TEST_CASE("the path of a resource field reads through its closures")
    {
        FakeComponent component;
        FakeResolver resolver;

        CHECK(PathOf(component).empty());

        Point(component, resolver, kCube);

        CHECK(PathOf(component) == kCube);
    }

    TEST_CASE("a resource edit resolves the new path, and undo and redo move between the two")
    {
        FakeComponent component;
        FakeResolver resolver;
        Point(component, resolver, kCube);
        const SharedPtr<FakeResource> cube = component.mesh.GetResource();
        REQUIRE(cube);

        UndoStack stack;
        stack.Execute(MakeEdit(component, resolver, kSphere));

        CHECK(component.mesh.GetPath() == kSphere);
        REQUIRE(component.mesh.IsResolved());
        CHECK(component.mesh.GetResource()->path == kSphere);
        const SharedPtr<FakeResource> sphere = component.mesh.GetResource();

        stack.Undo();
        CHECK(component.mesh.GetPath() == kCube);
        CHECK(component.mesh.GetResource() == cube);

        stack.Redo();
        CHECK(component.mesh.GetPath() == kSphere);
        CHECK(component.mesh.GetResource() == sphere);
    }

    TEST_CASE("a resource edit to an empty path clears the field, and undo restores it")
    {
        FakeComponent component;
        FakeResolver resolver;
        Point(component, resolver, kCube);
        const SharedPtr<FakeResource> cube = component.mesh.GetResource();

        const UniquePtr<PropertyEditCommand> command = MakeEdit(component, resolver, "");
        command->Execute();

        CHECK(component.mesh.GetPath().empty());
        CHECK_FALSE(component.mesh.IsResolved());

        command->Undo();

        CHECK(component.mesh.GetPath() == kCube);
        CHECK(component.mesh.GetResource() == cube);
    }

    TEST_CASE("a resource edit from an empty field undoes back to empty")
    {
        FakeComponent component;
        FakeResolver resolver;

        const UniquePtr<PropertyEditCommand> command = MakeEdit(component, resolver, kCube);
        command->Execute();
        CHECK(component.mesh.IsResolved());

        command->Undo();

        CHECK(component.mesh.GetPath().empty());
        CHECK_FALSE(component.mesh.IsResolved());
    }

    TEST_CASE("removing a component takes it off, and undo brings the same one back")
    {
        Scene scene("test");
        SharedPtr<Entity> entity = scene.CreateEntity();
        FakeComponent& added = entity->CreateComponent<FakeComponent>();
        entity->CreateComponent<OtherComponent>();
        added.intensity = 7.0f;
        Component* const original = &added;

        RemoveComponentCommand command(*entity, typeid(FakeComponent));

        command.Execute();
        CHECK_FALSE(entity->HasComponent<FakeComponent>());
        CHECK(entity->HasComponent<OtherComponent>());

        command.Undo();
        REQUIRE(entity->HasComponent<FakeComponent>());
        FakeComponent* restored = entity->GetComponent<FakeComponent>();
        CHECK(restored == original);
        CHECK(restored->intensity == 7.0f);

        command.Execute();
        CHECK_FALSE(entity->HasComponent<FakeComponent>());

        command.Undo();
        CHECK(entity->GetComponent<FakeComponent>() == original);
    }

    TEST_CASE("a removed component shuts down, and starts up again on undo")
    {
        Scene scene("test");
        SharedPtr<Entity> entity = scene.CreateEntity();
        FakeComponent& component = entity->CreateComponent<FakeComponent>();
        REQUIRE(component.initializeCount == 1);

        RemoveComponentCommand command(*entity, typeid(FakeComponent));
        command.Execute();
        CHECK(component.shutdownCount == 1);

        command.Undo();
        CHECK(component.initializeCount == 2);
    }

    TEST_CASE("earlier undo steps on a component's fields survive its removal")
    {
        Scene scene("test");
        SharedPtr<Entity> entity = scene.CreateEntity();
        FakeComponent& component = entity->CreateComponent<FakeComponent>();
        component.intensity = 1.0f;

        UndoStack stack;
        stack.Execute(MakeUnique<PropertyEditCommand>(&component, &IntensityField(), FieldValue(1.0f), FieldValue(5.0f)));
        stack.Execute(MakeUnique<RemoveComponentCommand>(*entity, typeid(FakeComponent)));
        CHECK_FALSE(entity->HasComponent<FakeComponent>());

        stack.Undo();
        REQUIRE(entity->HasComponent<FakeComponent>());
        CHECK(entity->GetComponent<FakeComponent>()->intensity == 5.0f);

        stack.Undo();
        CHECK(entity->GetComponent<FakeComponent>()->intensity == 1.0f);
    }
}
