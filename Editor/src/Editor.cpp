#include "Editor.h"
#include "EntryPoint.h"
#include "SmokeChecks.h"

#include <crtdbg.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{
    // main() takes no arguments, so these come from the CRT. Returns 0 when --smoke isn't given.
    int ParseSmokeFrames()
    {
        for (int i = 1; i < __argc; ++i)
        {
            if (std::strcmp(__argv[i], "--smoke") != 0)
            {
                continue;
            }

            const int frames = i + 1 < __argc ? std::atoi(__argv[i + 1]) : 0;
            if (frames <= 0)
            {
                std::fprintf(stderr, "--smoke needs a positive frame count\n");
                std::exit(2);
            }
            return frames;
        }
        return 0;
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
}

UniquePtr<Application> CreateApplication()
{
    const int smokeFrames = ParseSmokeFrames();
    if (smokeFrames > 0)
    {
        DisableCrashDialogs();
    }

    AppConfig appConfig;
    appConfig.renderAPI = GraphicsDevice::API::OpenGL;

    AppWindowConfig windowConfig;
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Axiom Editor";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = ApplicationWindowBackend::GLFW;

    appConfig.windowConfig = windowConfig;
    return MakeUnique<Editor>(appConfig, smokeFrames);
};

namespace axiom
{
    Editor::Editor(AppConfig appConfig, int smokeFrames)
    : Application(appConfig)
    , m_smokeFrames(smokeFrames)
    {
    }

    void Editor::OnResize(const WindowResizeEvent &event)
    {
    }

    void Editor::OnRegisterModules()
    {
        FileSubsystem& fileSystemModule = GetSubsystem<FileSubsystem>();

        fileSystemModule.Mount(MountPoints::Editor, AX_EDITOR_DIR);
        fileSystemModule.Mount(MountPoints::Project, AX_DEV_PROJECT_DIR); // read that from ini or "Open Project" dialog
    }

    void Editor::OnInitializeModules()
    {
        if (m_smokeFrames > 0)
        {
            // An automated run must not overwrite the user's layout.
            ImGui::GetIO().IniFilename = nullptr;
        }

        m_editorUI = MakeUnique<EditorUI>(GetSubsystem<RenderSubsystem>(), GetSubsystem<WorldSubsystem>(), GetSubsystem<FileSubsystem>());
        m_editorUI->OnInitialize();
    }

    void Editor::OnApplicationRun()
    {
        WorldSubsystem& worldSubsystem = GetSubsystem<WorldSubsystem>();
        worldSubsystem.LoadScene("project://Scenes/main.scene.json");

        if (m_smokeFrames > 0)
        {
            RunGizmoMathChecks();

            // With something selected the gizmo code path runs in the smoke frames too.
            for (Entity* entity : worldSubsystem.GetActiveScene().GetEntities())
            {
                if (entity->GetComponent<TransformComponent>())
                {
                    m_editorUI->GetSelection().Set(entity);
                    break;
                }
            }
        }
    }

    void Editor::OnRender()
    {
        m_editorUI->OnRender();

        if (m_smokeFrames > 0 && --m_smokeFrames == 0)
        {
            GetApplicationWindow().CloseWindow();
        }
    }

    void Editor::OnUpdate(float deltaTime)
    {
        m_editorUI->OnUpdate(deltaTime);
    }
}
