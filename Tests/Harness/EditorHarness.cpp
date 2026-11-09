#include "EditorHarness.h"

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
        // An automated run must not overwrite the user's layout.
        ImGui::GetIO().IniFilename = nullptr;
        Editor::OnInitializeModules();
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
}
