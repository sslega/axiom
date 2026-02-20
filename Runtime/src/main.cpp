#include "axiom/Core/Engine.h" 
#include "axiom/Core/Game.h"

extern axiom::Game* CreateGame();

int main()
{
    axiom::EngineConfig engineConfig;

    axiom::WindowDesc windowConfig;
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "My Axiom Game";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;

    axiom::GlfwWindow window(windowConfig);
    
    axiom::Engine engine(engineConfig, window);

    return engine.Run(CreateGame());
}