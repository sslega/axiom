#include "Editor.h"
#include "EntryPoint.h"

UniquePtr<Application> CreateApplication()
{

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
    return MakeUnique<Editor>(appConfig);
};

namespace axiom
{
    Editor::Editor(AppConfig appConfig)
    : Application(appConfig)
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
        m_editorUI = MakeUnique<EditorUI>(GetSubsystem<RenderSubsystem>(), GetSubsystem<WorldSubsystem>());
        m_editorUI->OnInitialize();
    }

    void Editor::OnApplicationRun()
    {
        GetSubsystem<WorldSubsystem>().LoadScene("project://Scenes/main.scene.json");
    }

    void Editor::OnRender()
    {
        m_editorUI->OnRender();
    }

    void Editor::OnUpdate(float deltaTime)
    {
        m_editorUI->OnUpdate(deltaTime);
    }
}
