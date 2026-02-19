#include <iostream>

#include "Axiom/Core/Engine.h"
using namespace ax;

int main() 
{
    WindowDesc windowDesc;
    windowDesc.width = 800;
    windowDesc.height = 600;
    windowDesc.fullscreen = false;
    windowDesc.title = "Yet Another Physics Engine";

    EngineConfig engineConfig;
    engineConfig.windowConfig = windowDesc;
    
    Engine engine(engineConfig);
    return engine.Run();
}