#include "axiom/core/Game.h"
#include "MyGame.h"

axiom::Game* CreateGame()
{
    return new MyGame();
}

MyGame::MyGame()
{
}