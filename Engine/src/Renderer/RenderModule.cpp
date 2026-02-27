#include "axiom/Renderer/RenderModule.h"
#include "axiom/Renderer/Renderer.h"
#include "Axiom/Core/Engine.h"
#include <iostream>
#include <cassert>

namespace axiom
{
    RenderModule::RenderModule(Engine& engine)
    : EngineModule(engine)
    {
    }

    bool RenderModule::Initialize()
    {
        bool bInitialized = EngineModule::Initialize();
        assert(bInitialized && "EngineModule initialization failed!");

        const RenderAPI renderAPI = m_engine.GetRenderAPI();            
        m_renderer = CreateRenderer(renderAPI);
        assert(m_renderer && "Failed to create renderer API");
        
        const IApplicationWindow& window = m_engine.GetApplicationWindow();
        return m_renderer->Initialize(window.GetNativeWindow()) && bInitialized;
    }

    void RenderModule::Shutdown()
    {
        m_renderer->Shutdown();
        m_renderer.reset();
    }

    void RenderModule::Update()
    {
        // nothing for now
    }

    void RenderModule::Render()
    {
        m_renderer->BeginFrame();
        m_renderer->EndFrame();
    }

}