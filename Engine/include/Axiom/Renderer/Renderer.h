#pragma once

#include "axiom/Core/EngineModule.h"

namespace axiom
{
    class Renderer: public EngineModule
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
    };
}