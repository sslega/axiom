#include "axiom/Core/Engine.h" 
#include "axiom/Core/Application.h"
#include "axiom/Platform/ApplicationWindow.h"

extern axiom::Application* CreateApplication();
int main()
{
    axiom::EngineConfig engineConfig;
    engineConfig.renderAPI = axiom::RenderAPI::OpenGL;

    axiom::ApplicationWindowConfig windowConfig;
    windowConfig.width = 800;
    windowConfig.height = 600;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = axiom::ApplicationWindowBackend::GLFW;

    axiom::Engine engine(engineConfig, windowConfig);

    return engine.Run(CreateApplication());
}