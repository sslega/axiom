#include "Renderer/RenderModule.h"
#include "Core/Application.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "RenderModule.h"
#include "Resources/ResourceModule.h"
#include "Scene/SceneModule.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/MeshComponent.h"
#include "Scene/TransformComponent.h"
#include "Core/Log.h"
#include "Renderer/Material.h"

namespace axiom
{
    RenderModule::RenderModule(Application& application)
    : ApplicationModule(application)
    {
    }

    void RenderModule::OnInitialize()
    {
        auto API = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();
        m_graphicsDevice = GraphicsDevice::Create(API, window);
    }

    void RenderModule::OnShutdown()
    {
    }

    void RenderModule::OnUpdate()
    {
    }

    void RenderModule::OnRender()
    {
        BeginScene();

        SceneModule* sceneModule = GetModule<SceneModule>();
        AX_ASSERT(sceneModule->HasActiveScene(), "No active Scene!");

        Scene* scene = sceneModule->GetActiveScene();
        auto meshComponents = scene->GetComponents<MeshComponent>();

        for (MeshComponent* meshComponent : meshComponents)
        {
            if (!meshComponent->IsVisible()) continue;

            const SharedPtr<Material> material = meshComponent->GetMaterial();
            const SharedPtr<MeshResource> mesh= meshComponent->GetMesh();
            if (!material || !mesh) continue;

            TransformComponent* transformComponent = meshComponent->GetEntity()->GetComponent<TransformComponent>();
            Matrix4 transform = transformComponent ? transformComponent->GetTransform() : Matrix4::Identity();
            
            // SharedPtr<VertexBuffer> VB = m_graphicsDevice->CreateVertexBuffer(*mesh.get());
            // SharedPtr<IndexBuffer> IB = m_graphicsDevice->CreateIndexBuffer(*mesh.get());
            
            auto buffers = GetOrCreateBuffers(mesh);
            // TODO: Improve shader/material binding
            Submit(buffers.vb, buffers.ib, material, transform);

        }

        EndScene();
    }

    GraphicsDevice& RenderModule::GetGraphicsDevice() const
    {
        return *m_graphicsDevice;
    }

    void RenderModule::BeginScene()
    {
        SceneModule* sceneModule = GetModule<SceneModule>();
        AX_ASSERT(sceneModule->HasActiveScene(), "No active Scene!");

        Scene* scene = sceneModule->GetActiveScene();
        Vector<CameraComponent*> cameras = scene->GetComponents<CameraComponent>();

        AX_ASSERT(!cameras.empty(), "No active Camera!");
        OrtographicCamera* camera = &cameras[0]->m_camera;
        

        m_sceneData.viewProjectionMatrix = camera->GetViewProjectionMatrix();
        m_graphicsDevice->SetClearColor(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
        m_graphicsDevice->Clear();
    }

    void RenderModule::EndScene()
    {
    }

    void RenderModule::Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Material>& material, const Matrix4& transform)
    {
        material->Bind();
        material->m_shader->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        material->m_shader->UploadUniform("u_Transform", transform);
        m_graphicsDevice->DrawIndexed(vb, ib);
    }

    void RenderModule::Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform)
    {
        shader->Bind();
        shader->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        shader->UploadUniform("u_Transform", transform);
        m_graphicsDevice->DrawIndexed(vb, ib);
    }

    SharedPtr<Shader> RenderModule::GetShader(const String path)
    {
        auto it = m_shaderCache.find(path);
        if(it == m_shaderCache.end())
        {
            m_shaderCache[path] = CreateShader(path);
        }
        it = m_shaderCache.find(path);
        return it->second;
    }

    SharedPtr<Shader> RenderModule::CreateShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>()->Load<ShaderResource>(path);
        return GetGraphicsDevice().CreateShader(*shaderResource);
    }

    RenderModule::MeshBuffers RenderModule::GetOrCreateBuffers(const SharedPtr<MeshResource> &mesh)
    {
        auto it = m_meshCache.find(mesh.get());
        if (it != m_meshCache.end()) return it->second;

        MeshBuffers buffers;
        buffers.vb = m_graphicsDevice->CreateVertexBuffer(*mesh);
        buffers.ib = m_graphicsDevice->CreateIndexBuffer(*mesh);
        m_meshCache[mesh.get()] = buffers;
        return buffers;
    }

    GraphicsDevice::API RenderModule::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }

    // RenderMesh* RenderModule::GetMesh(const SharedPtr<MeshResource> meshResource)
    // {
    //     auto it = m_meshes.find(meshResource);
    //     if(it != m_meshes.end())
    //     {
    //         return it->second.get();
    //     }
    //     UniquePtr<RenderMesh> mesh = CreateMesh(meshResource);
    //     RenderMesh* meshPtr = mesh.get();
    //     m_meshes[meshResource] = std::move(mesh);
    //     return meshPtr;
    // }

    // UniquePtr<RenderMesh> RenderModule::CreateMesh(const SharedPtr<MeshResource> meshResource)
    // {
    //     UniquePtr<RenderMesh> mesh;
    //     switch (m_renderApi)
    //     {
    //         case RenderAPI::OpenGL:
    //             mesh = MakeUnique<GLRenderMesh>(*meshResource.get());
    //             break;
    //         default:
    //             mesh = nullptr;
    //             AX_FATAL("Unknown RenderAPI");
    //     }
    //     return mesh;
    // }

    // void RenderModule::DestroyMesh(SharedPtr<MeshResource> meshResource)
    // {
    // }

    // RenderShader* RenderModule::GetShader(const SharedPtr<ShaderResource> shaderResource)
    // {
    //      auto it = m_shaders.find(shaderResource);
    //     if(it != m_shaders.end())
    //     {
    //         return it->second.get();
    //     }
    //     UniquePtr<RenderShader> shader = CreateShader(shaderResource);
    //     RenderShader* shaderPtr = shader.get();
    //     m_shaders[shaderResource] = std::move(shader);
    //     return shaderPtr;
    // }

    // UniquePtr<RenderShader> RenderModule::CreateShader(const SharedPtr<ShaderResource> shaderResource)
    // {
    //     UniquePtr<RenderShader> shader;
    //     switch (m_renderApi)
    //     {
    //         case RenderAPI::OpenGL:
    //             shader = MakeUnique<GLRenderShader>(*shaderResource.get());
    //             break;
    //         default:
    //             shader = nullptr;
    //             AX_FATAL("Unknown RenderAPI");

    //     }
    //     return shader;
    // }

    // void RenderModule::DestroyShader(SharedPtr<ShaderResource> shaderResource)
    // {
    // }
}
