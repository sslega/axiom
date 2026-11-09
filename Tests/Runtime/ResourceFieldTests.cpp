#include "RuntimeTest.h"

#include "Commands/PropertyEditCommand.h"
#include "Core/StringUtils.h"
#include "Reflection/Reflection.h"
#include "Resources/MeshResource.h"
#include "UI/ResourceCandidates.h"

#include <algorithm>
#include <cctype>

namespace axiom::tests
{
    TEST_SUITE("runtime")
    {
        namespace
        {
            bool Contains(const Vector<String>& paths, const String& path)
            {
                const String wanted = ToLower(path);
                return std::any_of(paths.begin(), paths.end(), [&](const String& entry) { return ToLower(entry) == wanted; });
            }

            String PathOf(const Component* component, const FieldDescriptor& field)
            {
                return std::get<ResourceValue>(field.get(component)).path;
            }

            const FieldDescriptor& MeshField()
            {
                const FieldDescriptor* field = Reflection::Get().Find(typeid(MeshComponent))->FindField("mesh");
                REQUIRE(field);
                return *field;
            }

            UniquePtr<PropertyEditCommand> MakeEdit(ResourceSubsystem& resources, Component* component, const FieldDescriptor& field, const String& path)
            {
                return MakeUnique<PropertyEditCommand>(component, &field, field.get(component), ResourceValue::Resolve(path, resources));
            }

            template <typename Fn>
            int ForEachResourceField(Scene& scene, Fn&& fn)
            {
                int resourceFields = 0;
                for (Entity* entity : scene.GetEntities())
                {
                    for (Component* component : entity->GetComponents())
                    {
                        const TypeDescriptor* type = Reflection::Get().Find(typeid(*component));
                        if (!type)
                        {
                            continue;
                        }
                        for (const FieldDescriptor& field : type->fields)
                        {
                            if (field.type == FieldType::Resource)
                            {
                                ++resourceFields;
                                fn(*entity, component, field);
                            }
                        }
                    }
                }
                return resourceFields;
            }

            doctest::String Describe(const Entity& entity, const FieldDescriptor& field)
            {
                return (entity.GetName() + "." + field.name).c_str();
            }

            String OtherMeshPath(FileSubsystem& files, ResourceSubsystem& resources, const String& current)
            {
                for (const String& candidate : ListResourceCandidates(files, resources, MeshField().resourceType))
                {
                    if (ToLower(candidate) != ToLower(current))
                    {
                        return candidate;
                    }
                }
                return "";
            }
        }

        TEST_CASE_FIXTURE(RuntimeTest, "the main scene runs frames with a mesh entity selected")
        {
            LoadMainSceneAndSelectMesh();

            RunFrames(10);

            CHECK(GetSelection().Get() != nullptr);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "every resource field lists its candidates, including the current one")
        {
            LoadMainScene();

            const int resourceFields = ForEachResourceField(GetScene(), [&](Entity& entity, Component* component, const FieldDescriptor& field)
            {
                INFO(Describe(entity, field));
                const Vector<String> candidates = ListResourceCandidates(GetFiles(), GetResources(), field.resourceType);
                const Vector<String> extensions = GetResources().GetExtensions(field.resourceType);

                CHECK_FALSE(candidates.empty());
                CHECK(Contains(candidates, PathOf(component, field)));
                for (const String& candidate : candidates)
                {
                    CHECK(Contains(extensions, Path(candidate).extension().string()));
                }
            });

            CHECK(resourceFields > 0);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "every listed candidate loads when picked, and undo restores the field")
        {
            LoadMainScene();

            const int resourceFields = ForEachResourceField(GetScene(), [&](Entity& entity, Component* component, const FieldDescriptor& field)
            {
                INFO(Describe(entity, field));
                const String current = PathOf(component, field);
                const SharedPtr<void> before = GetResources().ResolveResource(current);

                for (const String& candidate : ListResourceCandidates(GetFiles(), GetResources(), field.resourceType))
                {
                    const UniquePtr<PropertyEditCommand> command = MakeEdit(GetResources(), component, field, candidate);
                    command->Execute();
                    CHECK(PathOf(component, field) == candidate);
                    command->Undo();
                }

                CHECK(PathOf(component, field) == current);
                CHECK(GetResources().ResolveResource(current) == before);
            });

            CHECK(resourceFields > 0);
        }

        TEST_CASE_FIXTURE(RuntimeTest, "a resource pick undoes and redoes through the real resources")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();
            const String oldPath = mesh->mesh.GetPath();
            const SharedPtr<MeshResource> oldResource = mesh->mesh.GetResource();

            const String newPath = OtherMeshPath(GetFiles(), GetResources(), oldPath);
            REQUIRE_FALSE(newPath.empty());

            const UniquePtr<PropertyEditCommand> command = MakeEdit(GetResources(), mesh, MeshField(), newPath);
            command->Execute();
            CHECK(mesh->mesh.GetPath() == newPath);
            CHECK(mesh->mesh.IsResolved());
            const SharedPtr<MeshResource> newResource = mesh->mesh.GetResource();

            command->Undo();
            CHECK(mesh->mesh.GetPath() == oldPath);
            CHECK(mesh->mesh.GetResource() == oldResource);

            command->Execute();
            CHECK(mesh->mesh.GetPath() == newPath);
            CHECK(mesh->mesh.GetResource() == newResource);

            command->Undo();
        }

        TEST_CASE_FIXTURE(RuntimeTest, "a resource path in another case returns the same resource")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();

            String upperPath = mesh->mesh.GetPath();
            std::transform(upperPath.begin(), upperPath.end(), upperPath.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

            CHECK(GetResources().Load<MeshResource>(upperPath) == mesh->mesh.GetResource());
        }

        TEST_CASE_FIXTURE(RuntimeTest, "picking None clears the mesh across frames, and undo restores it")
        {
            Entity* entity = LoadMainSceneAndSelectMesh();
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();
            const String pathBefore = mesh->mesh.GetPath();
            const UniquePtr<PropertyEditCommand> command = MakeEdit(GetResources(), mesh, MeshField(), "");

            DuringNextFrame([&] { command->Execute(); });

            CHECK(mesh->mesh.GetPath().empty());
            CHECK_FALSE(mesh->mesh.IsResolved());
            RunFrames(3);

            DuringNextFrame([&] { command->Undo(); });

            CHECK(mesh->mesh.GetPath() == pathBefore);
            CHECK(mesh->mesh.IsResolved());
            RunFrames(1);
        }
    }
}
