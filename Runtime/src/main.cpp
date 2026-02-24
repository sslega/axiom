#include "axiom/Core/Engine.h" 
#include "axiom/Core/Game.h"

extern axiom::Game* CreateGame();

int main()
{
    axiom::EngineConfig engineConfig;

    axiom::ApplicationWindowDesc windowConfig;
    windowConfig.width = 1280;
    windowConfig.height = 720;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;

    axiom::Engine engine(engineConfig, windowConfig);

    // axiom::GlfwWindow window(windowConfig);
    
    // axiom::Engine engine(engineConfig, window);

    return engine.Run(CreateGame());
}