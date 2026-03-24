#include "Rendering/RenderModule.h"
#include "Rendering/RenderDevice.h"
#include "Rendering/MeshComponent.h"
#include "Rendering/RenderShader.h"
#include "Rendering/Material.h"
#include "Rendering/GL/GLRenderMesh.h"
#include "Rendering/GL/GLRenderShader.h"
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

        m_renderApi = m_application.GetRenderAPI();            
        m_renderer = CreateRenderDevice(m_renderApi);
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

            SharedPtr<MeshResource> meshResource = meshComponent->GetMesh();
            if(!meshResource) continue;

            SharedPtr<Material> material = meshComponent->GetMaterial();
            if(!material) continue;

            RenderMesh* mesh = GetMesh(meshResource);
            RenderShader* shader = GetShader(material->shader);
            
            shader->Bind();
            mesh->Bind();
            // Draw
            mesh->Unbind();
            shader->Unbind();

        }

        m_renderer->EndFrame();
    }

    RenderMesh* RenderModule::GetMesh(const SharedPtr<MeshResource> meshResource)
    {
        auto it = m_meshes.find(meshResource);
        if(it != m_meshes.end())
        {
            return it->second.get();
        }
        UniquePtr<RenderMesh> mesh = CreateMesh(meshResource);
        RenderMesh* meshPtr = mesh.get();
        m_meshes[meshResource] = std::move(mesh);
        return meshPtr;
    }

    UniquePtr<RenderMesh> RenderModule::CreateMesh(const SharedPtr<MeshResource> meshResource)
    {
        UniquePtr<RenderMesh> mesh;
        switch (m_renderApi)
        {
            case RenderAPI::OpenGL:
                mesh = MakeUnique<GLRenderMesh>(*meshResource.get());
                break;
            default:
                mesh = nullptr;
                AX_FATAL("Unknown RenderAPI");
        }
        return mesh;
    }

    void RenderModule::DestroyMesh(SharedPtr<MeshResource> meshResource)
    {
    }

    RenderShader* RenderModule::GetShader(const SharedPtr<ShaderResource> shaderResource)
    {
         auto it = m_shaders.find(shaderResource);
        if(it != m_shaders.end())
        {
            return it->second.get();
        }
        UniquePtr<RenderShader> shader = CreateShader(shaderResource);
        RenderShader* shaderPtr = shader.get();
        m_shaders[shaderResource] = std::move(shader);
        return shaderPtr;
    }

    UniquePtr<RenderShader> RenderModule::CreateShader(const SharedPtr<ShaderResource> shaderResource)
    {
        UniquePtr<RenderShader> shader;
        switch (m_renderApi)
        {
            case RenderAPI::OpenGL:
                shader = MakeUnique<GLRenderShader>(*shaderResource.get());
                break;
            default:
                shader = nullptr;
                AX_FATAL("Unknown RenderAPI");

        }
        return shader;
    }

    void RenderModule::DestroyShader(SharedPtr<ShaderResource> shaderResource)
    {
    }
}