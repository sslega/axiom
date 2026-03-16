#include "Rendering/RenderModule.h"
#include "Rendering/RenderDevice.h"
#include "Core/Application.h"
#include <iostream>
#include <cassert>

namespace axiom
{
    RenderModule::RenderModule(Application& engine)
    : EngineModule(engine)
    {
    }

    bool RenderModule::Initialize()
    {
        bool bInitialized = EngineModule::Initialize();
        assert(bInitialized && "EngineModule initialization failed!");

        const RenderAPI renderAPI = m_application.GetRenderAPI();            
        m_renderer = CreateRenderDevice(renderAPI);
        assert(m_renderer && "Failed to create renderer API");
        
        const IApplicationWindow& window = m_application.GetApplicationWindow();
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