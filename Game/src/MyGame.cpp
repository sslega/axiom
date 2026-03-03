#include "axiom/core/Application.h"
#include "axiom/Resources/Shader.h"
#include "MyGame.h"

#define RESOURCES "Z:/YetAnotherGameEngine/Engine/Resources"

axiom::Application* CreateApplication()
{
    return new MyGame();
};

MyGame::MyGame()
{
    //TODO: Find a way how to include build in resources in "deploy"
    axiom::String filePath(RESOURCES "/Shaders/VertexColor.glsl");
    axiom::Shader shader;
    bool loaded = shader.LoadFromFile(filePath);
}
