#pragma once

#include "Core/EngineModule.h"
#include "Renderer/Renderer.h"

namespace axiom
{
    class RenderModule : public EngineModule
    {
    public:
        RenderModule(Application& engine);

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

    private:
        UniquePtr<IRenderer> m_renderer;
    };
}