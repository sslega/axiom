#include "axiom/core/Application.h"
#include "MyGame.h"


axiom::Application* CreateApplication()
{
    return new MyGame();
};

MyGame::MyGame()
{
}
