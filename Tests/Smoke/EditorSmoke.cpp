#include "Editor.h"
#include "Commands/PropertyEditCommand.h"
#include "Commands/RemoveComponentCommand.h"
#include "Core/StringUtils.h"
#include "Reflection/Reflection.h"
#include "Resources/MeshResource.h"
#include "UI/ResourceCandidates.h"

#include <algorithm>
#include <cctype>
#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>

namespace
{
    int g_failedChecks = 0;

    // A failure both logs an [ERROR] line and fails the run through the exit code.
    void Check(bool condition, const String& what)
    {
        if (!condition)
        {
            Log::Error("Smoke check failed: {}", what);
            ++g_failedChecks;
        }
    }

    bool Contains(const Vector<String>& paths, const String& path)
    {
        const String wanted = ToLower(path);
        return std::any_of(paths.begin(), paths.end(), [&](const String& entry) { return ToLower(entry) == wanted; });
    }

    // A failed assert must end an automated run with an error code, not wait on a dialog nobody will click.
    void DisableCrashDialogs()
    {
        _set_error_mode(_OUT_TO_STDERR);
        _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

        for (int reportType : { _CRT_ERROR, _CRT_ASSERT })
        {
            _CrtSetReportMode(reportType, _CRTDBG_MODE_FILE);
            _CrtSetReportFile(reportType, _CRTDBG_FILE_STDERR);
        }
    }

    /// The real Editor, run unattended for a fixed number of frames.
    class SmokeEditor : public Editor
    {
    public:
        SmokeEditor(AppConfig appConfig, int frames)
        : Editor(appConfig)
        , m_framesLeft(frames)
        {
        }

    protected:
        void OnInitializeModules() override
        {
            // An automated run must not overwrite the user's layout.
            ImGui::GetIO().IniFilename = nullptr;
            Editor::OnInitializeModules();
        }

        void OnApplicationRun() override
        {
            Editor::OnApplicationRun();

            // With a mesh entity selected, the gizmo and the Inspector's resource fields run in the smoke frames too.
            for (Entity* entity : GetSubsystem<WorldSubsystem>().GetActiveScene().GetEntities())
            {
                if (entity->GetComponent<TransformComponent>() && entity->GetComponent<MeshComponent>())
                {
                    m_meshEntity = entity;
                    GetEditorUI().GetSelection().Set(entity);
                    break;
                }
            }
        }

        void OnRender() override
        {
            Editor::OnRender();

            // The removal and the cleared mesh stay in place across real frames, so rendering runs with them.
            ++m_frame;
            if (m_frame == kCheckListingFrame)
            {
                CheckListingForEveryResourceField();
                CheckResourceEditUndoRedo();
            }
            else if (m_frame == kRemoveFrame)
            {
                RemoveMeshComponent();
            }
            else if (m_frame == kRestoreFrame)
            {
                RestoreMeshComponent();
            }
            else if (m_frame == kClearFrame)
            {
                ClearMesh();
            }

            if (--m_framesLeft == 0)
            {
                RestoreClearedMesh();
                GetApplicationWindow().CloseWindow();
            }
        }

    private:
        static constexpr int kCheckListingFrame = 1;
        static constexpr int kRemoveFrame = 2;
        static constexpr int kRestoreFrame = 6;
        static constexpr int kClearFrame = 7;

        Vector<String> Candidates(const FieldDescriptor& field)
        {
            return ListResourceCandidates(GetSubsystem<FileSubsystem>(), GetSubsystem<ResourceSubsystem>(), field.resourceType);
        }

        const FieldDescriptor& MeshField() const
        {
            const FieldDescriptor* field = Reflection::Get().Find(typeid(MeshComponent))->FindField("mesh");
            AX_ASSERT(field, "MeshComponent has no mesh field");
            return *field;
        }

        static String PathOf(const Component* component, const FieldDescriptor& field)
        {
            return std::get<ResourceValue>(field.get(component)).path;
        }

        UniquePtr<PropertyEditCommand> MakeEdit(Component* component, const FieldDescriptor& field, const String& path)
        {
            return MakeUnique<PropertyEditCommand>(component, &field, field.get(component), ResourceValue::Resolve(path, GetSubsystem<ResourceSubsystem>()));
        }

        void CheckListingForEveryResourceField()
        {
            ResourceSubsystem& resources = GetSubsystem<ResourceSubsystem>();
            int resourceFields = 0;

            for (Entity* entity : GetSubsystem<WorldSubsystem>().GetActiveScene().GetEntities())
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
                        if (field.type != FieldType::Resource)
                        {
                            continue;
                        }
                        ++resourceFields;

                        const String name = entity->GetName() + "." + field.name;
                        const Vector<String> candidates = Candidates(field);
                        const Vector<String> extensions = resources.GetExtensions(field.resourceType);
                        const String current = PathOf(component, field);

                        Check(!candidates.empty(), name + ": the listing is empty");
                        Check(Contains(candidates, current), name + ": the listing is missing the current path " + current);
                        for (const String& candidate : candidates)
                        {
                            Check(Contains(extensions, Path(candidate).extension().string()), name + ": the listing holds " + candidate);
                        }

                        // Every listed file has to load: a real pick of each, undone straight away.
                        const SharedPtr<void> before = resources.ResolveResource(current);
                        for (const String& candidate : candidates)
                        {
                            const UniquePtr<PropertyEditCommand> command = MakeEdit(component, field, candidate);
                            command->Execute();
                            Check(PathOf(component, field) == candidate, name + ": didn't take " + candidate);
                            command->Undo();
                        }
                        Check(PathOf(component, field) == current, name + ": undo changed the path");
                        Check(resources.ResolveResource(current) == before, name + ": a pick and undo changed the resource");
                    }
                }
            }

            Check(resourceFields > 0, "the startup scene has no resource fields");
        }

        void CheckResourceEditUndoRedo()
        {
            ResourceSubsystem& resources = GetSubsystem<ResourceSubsystem>();
            MeshComponent* mesh = m_meshEntity->GetComponent<MeshComponent>();
            const String oldPath = mesh->mesh.GetPath();
            const SharedPtr<MeshResource> oldResource = mesh->mesh.GetResource();

            String upperPath = oldPath;
            std::transform(upperPath.begin(), upperPath.end(), upperPath.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
            Check(resources.Load<MeshResource>(upperPath) == oldResource, "a path in another case loaded a second copy");

            String newPath;
            for (const String& candidate : Candidates(MeshField()))
            {
                if (ToLower(candidate) != ToLower(oldPath))
                {
                    newPath = candidate;
                    break;
                }
            }
            Check(!newPath.empty(), "no second mesh to pick");

            const UniquePtr<PropertyEditCommand> command = MakeEdit(mesh, MeshField(), newPath);
            command->Execute();
            Check(mesh->mesh.GetPath() == newPath && mesh->mesh.IsResolved(), "resource edit didn't apply");
            const SharedPtr<MeshResource> newResource = mesh->mesh.GetResource();

            command->Undo();
            Check(mesh->mesh.GetPath() == oldPath, "undo didn't restore the old path");
            Check(mesh->mesh.GetResource() == oldResource, "undo didn't restore the same resource");

            command->Execute();
            Check(mesh->mesh.GetPath() == newPath && mesh->mesh.GetResource() == newResource, "redo didn't reapply the pick");

            command->Undo();
        }

        void RemoveMeshComponent()
        {
            m_removedComponent = m_meshEntity->GetComponent<MeshComponent>();
            m_removeCommand = MakeUnique<RemoveComponentCommand>(*m_meshEntity, typeid(MeshComponent));
            m_removeCommand->Execute();
            Check(!m_meshEntity->HasComponent<MeshComponent>(), "the removed component is still on the entity");
        }

        void RestoreMeshComponent()
        {
            m_removeCommand->Undo();
            Check(m_meshEntity->GetComponent<MeshComponent>() == m_removedComponent, "undo didn't bring back the same component");

            m_removeCommand->Execute();
            Check(!m_meshEntity->HasComponent<MeshComponent>(), "redo didn't remove the component");

            m_removeCommand->Undo();
            Check(m_meshEntity->GetComponent<MeshComponent>() == m_removedComponent, "second undo lost the component");
        }

        void ClearMesh()
        {
            MeshComponent* mesh = m_meshEntity->GetComponent<MeshComponent>();
            m_pathBeforeClear = mesh->mesh.GetPath();
            m_clearCommand = MakeEdit(mesh, MeshField(), "");
            m_clearCommand->Execute();
            Check(mesh->mesh.GetPath().empty() && !mesh->mesh.IsResolved(), "None didn't clear the field");
        }

        void RestoreClearedMesh()
        {
            if (!m_clearCommand)
            {
                return;
            }
            m_clearCommand->Undo();
            MeshComponent* mesh = m_meshEntity->GetComponent<MeshComponent>();
            Check(mesh->mesh.GetPath() == m_pathBeforeClear && mesh->mesh.IsResolved(), "undoing None didn't restore the mesh");
        }

        int m_framesLeft;
        int m_frame = 0;
        Entity* m_meshEntity = nullptr;

        Component* m_removedComponent = nullptr;
        UniquePtr<RemoveComponentCommand> m_removeCommand;

        String m_pathBeforeClear;
        UniquePtr<PropertyEditCommand> m_clearCommand;
    };
}

int main(int argc, char** argv)
{
    const int frames = argc == 2 ? std::atoi(argv[1]) : 0;
    if (frames <= 0)
    {
        std::fprintf(stderr, "usage: EditorSmoke <frame count>\n");
        return 2;
    }

    DisableCrashDialogs();

    // Owned through the base class, like the real entry point, so this run exercises the real shutdown.
    UniquePtr<Application> application = MakeUnique<SmokeEditor>(Editor::DefaultConfig(), frames);
    const int exitCode = application->Run();
    return exitCode != 0 ? exitCode : (g_failedChecks == 0 ? 0 : 1);
}
