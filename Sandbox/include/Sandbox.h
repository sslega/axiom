#pragma once

#include "Axiom/Core/Application.h"

class Sandbox : public axiom::Application
{
public:
    Sandbox(axiom::AppConfig appConfig);
    ~Sandbox() = default;

protected:
    virtual void OnApplicationStart() override;
};

