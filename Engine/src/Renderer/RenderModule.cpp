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
#include "Core/Types.h"
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

        Vector<RenderCommand> renderCommands;
        // 1. Collect
        for(MeshComponent* meshComponent : meshComponents)
        {
            if (!meshComponent->IsVisible()) continue;
            if (!meshComponent->GetMaterial() || !meshComponent->GetMesh()) continue;
            TransformComponent* tc = meshComponent->GetEntity()->GetComponent<TransformComponent>();
            renderCommands.push_back({ meshComponent->GetMesh(), meshComponent->GetMaterial(), tc ? tc->GetTransform() : Matrix4::Identity() });
        }

        // 2. Group by (mesh, material)
        PairMap<InstanceGroupKey, Vector<Matrix4>> instanceGroups;
        for (auto& renderCommand : renderCommands)
        {
            InstanceGroupKey groupKey{ renderCommand.mesh.get(), renderCommand.material.get() };
            instanceGroups[groupKey].push_back(renderCommand.transform);
        }
        
        // 3. Dispatch
        for(auto& [key, transforms] : instanceGroups)
        {
            // need SharedPtrs back — find first matching command
            auto it = std::find_if(renderCommands.begin(), renderCommands.end(), [&](const RenderCommand& c){
                return c.mesh.get() == key.first && c.material.get() == key.second;
            });
            auto buffers  = GetOrCreateBuffers(it->mesh);
            auto material = it->material;
            if (transforms.size() > 1)
            {
                SubmitInstanced(buffers, material, transforms);
            }
            else
            {
                Submit(buffers.vb, buffers.ib, material, transforms[0]);
            }
        }
        
        // for (MeshComponent* meshComponent : meshComponents)
        // {
        //     if (!meshComponent->IsVisible()) continue;

        //     const SharedPtr<Material> material = meshComponent->GetMaterial();
        //     const SharedPtr<MeshResource> mesh= meshComponent->GetMesh();
        //     if (!material || !mesh) continue;

        //     TransformComponent* transformComponent = meshComponent->GetEntity()->GetComponent<TransformComponent>();
        //     Matrix4 transform = transformComponent ? transformComponent->GetTransform() : Matrix4::Identity();
            
        //     // SharedPtr<VertexBuffer> VB = m_graphicsDevice->CreateVertexBuffer(*mesh.get());
        //     // SharedPtr<IndexBuffer> IB = m_graphicsDevice->CreateIndexBuffer(*mesh.get());
            
        //     auto buffers = GetOrCreateBuffers(mesh);
        //     // TODO: Improve shader/material binding
        //     Submit(buffers.vb, buffers.ib, material, transform);

        // }

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
        material->SetUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        material->SetUniform("u_Transform", transform);
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
            auto shader = CreateShader(path);
            m_shaderCache[path] = shader;

            auto instancedShader = CreateInstancedShader(path);
            m_instancedShaderCache[shader.get()] = instancedShader;
            it = m_shaderCache.find(path);
        }
        return it->second;
    }

    SharedPtr<Shader> RenderModule::CreateShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>()->Load<ShaderResource>(path);
        return GetGraphicsDevice().CreateShader(*shaderResource);
    }

    SharedPtr<Shader> RenderModule::CreateInstancedShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>()->Load<ShaderResource>(path);
        // Inject #define after the #version line
        const String& vertSrc = shaderResource->GetVertexSource();
        
        size_t versionPos = vertSrc.find("#version");
        size_t newline    = vertSrc.find('\n', versionPos);   // newline AFTER #version line

        String instancedVert = vertSrc.substr(0, newline + 1)
                    + "#define INSTANCED 1\n"
                    + vertSrc.substr(newline + 1);

        return GetGraphicsDevice().CreateShader(instancedVert, shaderResource->GetFragmentSource());
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

    SharedPtr<Shader> RenderModule::GetOrCreateInstancedShader(const SharedPtr<Shader> &shader)
    {
        auto it = m_instancedShaderCache.find(shader.get());
        return it != m_instancedShaderCache.end() ? it->second : nullptr;
    }

    void RenderModule::SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Material>& material, const Vector<Matrix4>& transforms)
    {
        uint32 byteSize = static_cast<uint32>(transforms.size() * sizeof(Matrix4));
        auto cacheKey = std::make_pair(buffers.vb.get(), material.get());
        auto it = m_instanceBufferCache.find(cacheKey);
        if (it == m_instanceBufferCache.end())
        {
            auto instanceBuffer = m_graphicsDevice->CreateDynamicVertexBuffer(byteSize);
                instanceBuffer->SetLayout({
                { ShaderDataType::Float4, "a_InstanceTransform0" },
                { ShaderDataType::Float4, "a_InstanceTransform1" },
                { ShaderDataType::Float4, "a_InstanceTransform2" },
                { ShaderDataType::Float4, "a_InstanceTransform3" },
            });
            m_instanceBufferCache[cacheKey] = instanceBuffer;
            it = m_instanceBufferCache.find(cacheKey);
        }

        SharedPtr<VertexBuffer>& instanceBuffer = it->second;
        instanceBuffer->SetData(transforms.data(), byteSize);

        material->Bind();  // binds regular shader, uploads uniforms + textures

        // Override with instanced shader variant
        SharedPtr<Shader> instancedShader = GetOrCreateInstancedShader(material->GetShader());
        if (instancedShader)
        {
            instancedShader->Bind();
            instancedShader->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        }
        else
        {
            material->GetShader()->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        }
        
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
    }

    void RenderModule::SubmitBatched(const SharedPtr<Material>& material, const Vector<RenderCommand>& commands)
    {
    }

    GraphicsDevice::API RenderModule::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }
}
