#include "EditorHarness.h"
#include "TestComponents.h"
#include "Reflection/ReflectionSubsystem.h"
#include "Core/FileSubsystem.h"

#include <utility>

namespace axiom::tests
{
    namespace
    {
        UniquePtr<EditorHarness> s_editor;
        bool s_windowVisible = false;
    }

    EditorHarness::EditorHarness(AppConfig appConfig)
    : Editor(appConfig)
    {
    }

    EditorHarness& EditorHarness::GetOrBoot()
    {
        if (!s_editor)
        {
            AppConfig appConfig = Editor::DefaultConfig();
            appConfig.windowConfig.visible = s_windowVisible;

            s_editor = MakeUnique<EditorHarness>(appConfig);
            s_editor->Startup();
        }
        return *s_editor;
    }

    void EditorHarness::ShutdownIfBooted()
    {
        if (s_editor)
        {
            s_editor->Shutdown();
            s_editor.reset();
        }
    }

    void EditorHarness::SetWindowVisible(bool visible)
    {
        s_windowVisible = visible;
    }

    void EditorHarness::QueueDuringRender(std::function<void()> callback)
    {
        m_duringRender = std::move(callback);
    }

    void EditorHarness::OnInitializeModules()
    {
        Editor::OnInitializeModules();

        // After the base call, which points ImGui at the user's layout: a run must never write it.
        // Every boot starts from the default instead, so no case depends on the user's layout.
        ImGui::GetIO().IniFilename = nullptr;
        GetEditorUI().RequestLayoutReset();
    }

    void EditorHarness::OnRender()
    {
        Editor::OnRender();

        if (m_duringRender)
        {
            // Moved out first, so a callback can queue the next one.
            std::function<void()> callback = std::move(m_duringRender);
            m_duringRender = nullptr;
            callback();
        }
    }
    
    void EditorHarness::OnRegisterModules()
    {
        Editor::OnRegisterModules();

        GetSubsystem<ReflectionSubsystem>().AddRegistrar(&RegisterTestComponents);
        GetSubsystem<FileSubsystem>().Mount("tests", AX_TESTS_DIR);
    }
}
