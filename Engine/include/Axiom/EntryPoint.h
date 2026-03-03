#include "Core/Engine.h" 
#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"

extern axiom::EngineConfig CreateEngineConfig();
extern axiom::ApplicationWindowConfig CreateApplicationWindowConfig();
extern axiom::Application* CreateApplication();

int main()
{
    axiom::EngineConfig engineConfig = CreateEngineConfig();
    axiom::ApplicationWindowConfig windowConfig = CreateApplicationWindowConfig();

    axiom::Engine engine(engineConfig, windowConfig);

    return engine.Run(CreateApplication());
}