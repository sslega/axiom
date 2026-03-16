#include "Axiom/EntryPoint.h"
#include "Axiom/Core/Application.h"
#include "Axiom/Core/FileSystem.h"
#include "Axiom/Core/Types.h"
#include "Axiom/Rendering/GLShader.h"


#include "Sandbox.h"
#include "TriangleMesh.h"

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
    
    return MakeUnique<Sandbox>(appConfig);
};

Sandbox::Sandbox(AppConfig appConfig)
: Application(appConfig)
{
}

void Sandbox::OnApplicationStart()
{
    // GLShader shader = GLShader(GetShaderPath("VertexColor.glsl"));
    // bool loaded = shader.Load();

    // CubeMesh cube = CubeMesh();

    TriangleMesh triangle = TriangleMesh();
}