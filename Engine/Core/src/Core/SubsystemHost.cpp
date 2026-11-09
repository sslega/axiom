#include "Core/SubsystemHost.h"

namespace axiom
{
    void SubsystemHost::InitializeSubsystems()
    {
        Log::Info("Initializing engine modules...");
        for (auto& id : m_subsystemOrder)
        {
            Log::Info("Initializing module: {}", id.name());
            m_applicationSubsystems[id]->Initialize();
        }
    }

    void SubsystemHost::ShutdownSubsystems()
    {
        Log::Info("Shutting down engine modules...");
        for (auto it = m_subsystemOrder.rbegin(); it != m_subsystemOrder.rend(); ++it)
        {
            Log::Info("Shutting down module: {}", it->name());
            m_applicationSubsystems[*it]->Shutdown();
        }
    }

    void SubsystemHost::UpdateSubsystems(float deltaTime)
    {
        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->Update(deltaTime);
        }
    }

    void SubsystemHost::BeginFrameSubsystems()
    {
        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->BeginFrame();
        }
    }

    void SubsystemHost::RenderSubsystems()
    {
        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->Render();
        }
    }

    void SubsystemHost::EndFrameSubsystems()
    {
        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->EndFrame();
        }
    }

    void SubsystemHost::DestroySubsystems()
    {
        m_applicationSubsystems.clear();
        m_subsystemOrder.clear();
    }
}
