#include "Axiom/Core/Application.h"
#include "Axiom/Core/FileSystem.h"
#include "Axiom/Core/Types.h"
#include "Axiom/Resources/Shader.h"
#include "Axiom/EntryPoint.h"

#include "MyGame.h"


axiom::EngineConfig CreateEngineConfig()
{
    axiom::EngineConfig engineConfig;
    engineConfig.renderAPI = axiom::RenderAPI::OpenGL;
    return engineConfig;
}

axiom::ApplicationWindowConfig CreateApplicationWindowConfig()
{
    axiom::ApplicationWindowConfig windowConfig;
    windowConfig.width = 800;
    windowConfig.height = 600;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = axiom::ApplicationWindowBackend::GLFW;
    return windowConfig;
}

axiom::Application* CreateApplication()
{
    return new MyGame();
};

MyGame::MyGame()
{
    axiom::Path shaderPath = axiom::GetShaderPath("VertexColor.glsl");
    axiom::Shader shader = axiom::Shader(shaderPath);
    bool loaded = shader.Load();
}
