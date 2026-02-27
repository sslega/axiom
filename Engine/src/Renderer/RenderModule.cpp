#include "axiom/Renderer/RenderModule.h"
#include "axiom/Renderer/Renderer.h"
#include "Axiom/Core/Engine.h"
#include <iostream>

namespace axiom
{
    RenderModule::RenderModule()
    {
    }

    bool RenderModule::Initialize(Engine &engine)
    {
        if (!EngineModule::Initialize(engine))
        {
            return false;
        }
        RenderAPI renderAPI = m_engine->GetRenderAPI();            
        m_renderer = CreateRenderer(renderAPI);
        if (!m_renderer)
        {
            printf("Failed to create renderer for API: %s\n", renderAPI);
            return false;
        }
        
        IApplicationWindow* window = m_engine->GetApplicationWindow();
        void* native = window->GetNativeWindow();

        return m_renderer->Initialize(native);
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