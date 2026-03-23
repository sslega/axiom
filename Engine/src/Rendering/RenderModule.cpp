#include "Rendering/RenderModule.h"
#include "Rendering/RenderDevice.h"
#include "Rendering/MeshComponent.h"
#include "Rendering/RenderShader.h"
#include "Rendering/Material.h"
#include "Core/Application.h"
#include "Core/Assert.h"
#include "Scene/SceneModule.h"
#include <iostream>
#include <cassert>

namespace axiom
{
    RenderModule::RenderModule(Application& application)
    : ApplicationModule(application)
    {
    }

    bool RenderModule::Initialize()
    {
        m_sceneModule = GetApp().GetModule<SceneModule>();
        AX_ASSERT(m_sceneModule, "Scene Module not registered");
        
        bool bInitialized = ApplicationModule::Initialize();
        assert(bInitialized && "ApplicationModule initialization failed!");

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
        Scene* activeScene = m_sceneModule->GetActiveScene();
        if (!activeScene) return;

        m_meshComponents.clear();
        activeScene->GetComponents<MeshComponent>(m_meshComponents);
        
        m_renderer->BeginFrame();

        for (MeshComponent* meshComponent : m_meshComponents)
        {
            if(!meshComponent->IsVisible()) continue;

            SharedPtr<MeshResource> mesh = meshComponent->GetMesh();
            if(!mesh) continue;

            SharedPtr<Material> material = meshComponent->GetMaterial();
            if(!material) continue;

            RenderMesh* proxy = GetProxy(mesh);
            SharedPtr<RenderShader> shader = material->shader;
            
            shader->Bind();
            proxy->Bind();
            // Draw
            proxy->Unbind();
            shader->Unbind();

        }

        m_renderer->EndFrame();
    }

    RenderMesh* RenderModule::GetProxy(SharedPtr<MeshResource> mesh)
    {
        return nullptr;
    }

    RenderMesh* RenderModule::CreateProxy(SharedPtr<MeshResource> mesh)
    {
        return nullptr;
    }

    void RenderModule::DestroyProxy(SharedPtr<MeshResource> mesh)
    {
    }
}
