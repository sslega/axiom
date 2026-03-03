#pragma once

#include "Core/Engine.h"
#include "Core/EngineModule.h"

namespace axiom
{
    class RenderModule : public EngineModule
    {
    public:
        RenderModule(Engine& engine);

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

    private:
        UniquePtr<IRenderer> m_renderer;
    };
}