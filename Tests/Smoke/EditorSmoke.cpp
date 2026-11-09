#include "Editor.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>

namespace
{
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

            // With something selected the gizmo code path runs in the smoke frames too.
            for (Entity* entity : GetSubsystem<WorldSubsystem>().GetActiveScene().GetEntities())
            {
                if (entity->GetComponent<TransformComponent>())
                {
                    GetEditorUI().GetSelection().Set(entity);
                    break;
                }
            }
        }

        void OnRender() override
        {
            Editor::OnRender();

            if (--m_framesLeft == 0)
            {
                GetApplicationWindow().CloseWindow();
            }
        }

    private:
        int m_framesLeft;
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
    return application->Run();
}
