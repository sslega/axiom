#pragma once

#include "axiom/Core/Engine.h"
#include "axiom/Core/EngineModule.h"

namespace axiom
{
    class RenderModule : public EngineModule
    {
    public:
        RenderModule();

        bool Initialize(Engine& engine) override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

    private:
        UniquePtr<IRenderer> m_renderer;
    };
}