#pragma once

#include "axiom/core/Application.h"

class MyGame : public axiom::Application
{
public:
    MyGame(axiom::AppConfig appConfig);
    ~MyGame() = default;
};

