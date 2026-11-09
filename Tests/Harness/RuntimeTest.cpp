#include "RuntimeTest.h"

#include "Commands/CreateEntityCommand.h"
#include "Commands/UndoStack.h"
#include "Selection.h"

#include <utility>

namespace axiom::tests
{
    namespace
    {
        constexpr const char* kMainScene = "project://Scenes/main.scene.json";
    }

    RuntimeTest::RuntimeTest()
    : m_errorsAtStart(Log::GetErrorCount())
    , m_editor(EditorHarness::GetOrBoot())
    {
        GetUI().NewScene();
    }

    RuntimeTest::~RuntimeTest()
    {
        const uint32 logged = Log::GetErrorCount() - m_errorsAtStart;
        CHECK_MESSAGE(logged == m_expectedErrors, "logged errors in this case; declare expected ones with ExpectLoggedErrors()");
    }

    void RuntimeTest::RunFrames(int count)
    {
        for (int i = 0; i < count; ++i)
        {
            m_editor.RunFrame();
        }
    }

    void RuntimeTest::DuringNextFrame(std::function<void()> fn)
    {
        m_editor.QueueDuringRender(std::move(fn));
        m_editor.RunFrame();
    }

    void RuntimeTest::LoadMainScene()
    {
        REQUIRE(GetUI().LoadScene(kMainScene));
    }

    Scene& RuntimeTest::GetScene()
    {
        return GetUI().GetActiveScene();
    }

    Selection& RuntimeTest::GetSelection()
    {
        return GetUI().GetSelection();
    }

    UndoStack& RuntimeTest::GetUndoStack()
    {
        return GetUI().GetUndoStack();
    }

    void RuntimeTest::Undo()
    {
        GetUndoStack().Undo();
    }

    void RuntimeTest::Redo()
    {
        GetUndoStack().Redo();
    }

    Entity* RuntimeTest::CreateSelectedEntity()
    {
        UniquePtr<CreateEntityCommand> command = MakeUnique<CreateEntityCommand>(GetScene());
        Entity* entity = command->GetEntity();
        GetUndoStack().Execute(std::move(command));
        GetSelection().Set(entity);
        return entity;
    }

    Entity* RuntimeTest::LoadMainSceneAndSelectMesh()
    {
        LoadMainScene();

        for (Entity* entity : GetScene().GetEntities())
        {
            if (entity->GetComponent<TransformComponent>() && entity->GetComponent<MeshComponent>())
            {
                GetSelection().Set(entity);
                return entity;
            }
        }

        FAIL("the main scene has no entity with a transform and a mesh");
        return nullptr;
    }

    void RuntimeTest::ExpectLoggedErrors(uint32 count)
    {
        m_expectedErrors = count;
    }

    ResourceSubsystem& RuntimeTest::GetResources()
    {
        return m_editor.GetSubsystem<ResourceSubsystem>();
    }

    FileSubsystem& RuntimeTest::GetFiles()
    {
        return m_editor.GetSubsystem<FileSubsystem>();
    }

    EditorUI& RuntimeTest::GetUI()
    {
        return m_editor.GetEditorUI();
    }
}
