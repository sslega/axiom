#include "Axiom/Core/Application.h"
#include "Axiom/Core/FileSystem.h"
#include "Axiom/Core/Types.h"
#include "Axiom/Resources/Shader.h"
#include "Axiom/EntryPoint.h"

#include "MyGame.h"

using namespace axiom;


UniquePtr<Application> CreateApplication()
{
    AppConfig appConfig;
    appConfig.renderAPI = axiom::RenderAPI::OpenGL;
    
    AppWindowConfig windowConfig;
    windowConfig.width = 800;
    windowConfig.height = 600;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = ApplicationWindowBackend::GLFW;

    appConfig.windowConfig = windowConfig;
    
    return MakeUnique<MyGame>(appConfig);
};

MyGame::MyGame(AppConfig appConfig)
: Application(appConfig)
{
    // Shader shader = Shader(GetShaderPath("VertexColor.glsl"));
    // bool loaded = shader.Load();
}
