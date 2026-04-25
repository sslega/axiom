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
#include "FrameBuffer.h"
#include <imgui.h>
#include "Geometry/Quad.h"

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

        FramebufferSpec fbSpec;
        fbSpec.width = window.GetWidth();
        fbSpec.height = window.GetHeight();
        m_frameBuffer = m_graphicsDevice->CreateFrameBuffer(fbSpec);

        Quad quad;
        m_screenQuadVB = m_graphicsDevice->CreateVertexBuffer(quad);
        m_screenQuadIB = m_graphicsDevice->CreateIndexBuffer(quad);
        m_screenQuadShader = GetShader("engine://Shaders/FullScreen.glsl");
    }

    void RenderModule::OnShutdown()
    {
    }

    void RenderModule::OnUpdate(float deltaTime)
    {
    }

    void RenderModule::OnBeginFrame()
    {
        ResetDebugDrawCounters();
        BeginScene();
    }

    void RenderModule::OnRender()
    {    
        auto now = std::chrono::steady_clock::now();
        m_dt = std::chrono::duration<float>(now - m_lastRenderTime).count();
        m_lastRenderTime = now;

        SceneModule& sceneModule = GetModule<SceneModule>();

        Scene& scene = sceneModule.GetActiveScene();
        auto meshComponents = scene.GetComponents<MeshComponent>();

        Vector<RenderCommand> renderCommands;
        // 1. Collect
        for(MeshComponent* meshComponent : meshComponents)
        {
            if (!meshComponent->IsVisible()) continue;
            if (!meshComponent->GetMaterial() || !meshComponent->GetMesh()) continue;
            TransformComponent* tc = meshComponent->GetEntity().GetComponent<TransformComponent>();
            renderCommands.push_back({ meshComponent->GetMesh(), meshComponent->GetMaterial(), tc ? tc->GetTransform() : Matrix4::Identity() });
        }

        // 2. Depth Pre-Pass
        m_graphicsDevice->SetColorWriteEnabled(false);
        m_graphicsDevice->SetDepthWriteEnabled(true);

        UnorderedMap<MeshResource*, Vector<Matrix4>> depthGroups;
        for (const auto& cmd : renderCommands)
            depthGroups[cmd.mesh.get()].push_back(cmd.transform);

        for (const auto& [meshPtr, transforms] : depthGroups)
        {
            auto it = std::find_if(renderCommands.begin(), renderCommands.end(),
                [meshPtr](const RenderCommand& c) { return c.mesh.get() == meshPtr; });
            auto buffers = GetOrCreateBuffers(it->mesh);
            auto& materialShader = it->material->GetShader();

            if (transforms.size() > 1)
                SubmitInstanced(buffers, GetOrCreateDepthPassInstancedShader(materialShader), transforms);
            else
                Submit(buffers.vb, buffers.ib, GetOrCreateDepthPassShader(materialShader), transforms[0]);
        }

        m_graphicsDevice->SetColorWriteEnabled(true);
        m_graphicsDevice->SetDepthWriteEnabled(false);
        m_graphicsDevice->SetDepthFunction(DepthFunction::LessEqual);

        // 3. Group by (mesh, material)
        PairMap<InstanceGroupKey, Vector<Matrix4>> instanceGroups;
        for (auto& renderCommand : renderCommands)
        {
            InstanceGroupKey groupKey{ renderCommand.mesh.get(), renderCommand.material.get() };
            instanceGroups[groupKey].push_back(renderCommand.transform);
        }
        
        // 4. Dispatch Intanced meshesh, collect batch candidates
        UnorderedMap<Material*, Vector<RenderCommand>> batchCandidates;
        for (auto& [key, transforms] : instanceGroups)
        {
            auto it = std::find_if(renderCommands.begin(), renderCommands.end(), [&](const RenderCommand& c){
                return c.mesh.get() == key.first && c.material.get() == key.second;
            });
            auto buffers = GetOrCreateBuffers(it->mesh);
            if (transforms.size() > 1)
            {
                SubmitInstanced(buffers, it->material, transforms);
            }
            else
            {
                batchCandidates[key.second].push_back({ it->mesh, it->material, transforms[0] });
            }
                
        }

        // 5. Draw batch and everything else
        for (auto& [matPtr, cmds] : batchCandidates)
        {
            if (cmds.size() > 1)
                SubmitBatched(cmds[0].material, cmds);
            else
            {
                auto buffers = GetOrCreateBuffers(cmds[0].mesh);
                Submit(buffers.vb, buffers.ib, cmds[0].material, cmds[0].transform);
            }
        }

        OnGUI();
        RenderToScreen();

    }

    void RenderModule::OnEndFrame()
    {
        EndScene(); 
    }

    GraphicsDevice& RenderModule::GetGraphicsDevice() const
    {
        return *m_graphicsDevice;
    }

    void RenderModule::BeginScene()
    {
        m_frameBuffer->Bind();

        SceneModule& sceneModule = GetModule<SceneModule>();

        Scene& scene = sceneModule.GetActiveScene();
        Vector<CameraComponent*> cameras = scene.GetComponents<CameraComponent>();

        AX_ASSERT(!cameras.empty(), "No active Camera!");
        CameraComponent* cameraComponent = cameras[0];
        TransformComponent* transform = cameraComponent->GetEntity().GetComponent<TransformComponent>();
        cameraComponent->SetAspectRatio(GetApp().GetApplicationWindow().GetAspectRatio());
        Matrix4 viewMatrix = transform ? Camera::GetViewMatrix(transform->position, transform->rotation) : Matrix4::Identity();
        m_sceneData.viewProjectionMatrix = cameraComponent->GetProjectionMatrix() * viewMatrix;
        m_graphicsDevice->SetDepthWriteEnabled(true);
        m_graphicsDevice->SetDepthFunction(DepthFunction::Less);
        m_graphicsDevice->SetClearColor(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
        m_graphicsDevice->Clear();
    }

    void RenderModule::EndScene()
    {
        m_graphicsDevice->SwapBuffers();
    }

    void RenderModule::OnGUI()
    {
        uint8 fps = static_cast<uint8>(1.0f / m_dt);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("##debug", nullptr,
            ImGuiWindowFlags_NoDecoration  |
            ImGuiWindowFlags_NoNav         |
            ImGuiWindowFlags_NoMove        |
            ImGuiWindowFlags_NoInputs      |
            ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("FPS: %d", fps);
        ImGui::Text("Draw calls: %d", GetDrawCallCount());
        ImGui::Text("Instanced calls: %d", GetInstanceCallCount());
        ImGui::Text("Instanced objects: %d", GetInstanceObjectCount());
        ImGui::Text("Batched calls: %d", GetBatchCallCount());
        ImGui::Text("Batched objects: %d", GetBatchObjectCount());
        ImGui::End();
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

            auto depthShader = CreateDepthPassShader(path);
            m_depthPassShaderCache[shader.get()] = depthShader;

            auto depthInstancedShader = CreateDepthPassInstancedShader(path);
            m_depthPassInstancedShaderCache[shader.get()] = depthInstancedShader;
        }
        return it->second;
    }

    SharedPtr<Shader> RenderModule::CreateShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>().Load<ShaderResource>(path);
        return GetGraphicsDevice().CreateShader(*shaderResource);
    }

    SharedPtr<Shader> RenderModule::CreateInstancedShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>().Load<ShaderResource>(path);
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

    SharedPtr<Shader> RenderModule::CreateDepthPassShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>().Load<ShaderResource>(path);
        const String depthFrag = "#version 330 core\nvoid main() {}\n";
        return GetGraphicsDevice().CreateShader(shaderResource->GetVertexSource(), depthFrag);
    }

    SharedPtr<Shader> RenderModule::CreateDepthPassInstancedShader(const String path)
    {
        auto shaderResource = GetModule<ResourceModule>().Load<ShaderResource>(path);
        const String& vertSrc = shaderResource->GetVertexSource();
        size_t newline = vertSrc.find('\n', vertSrc.find("#version"));
        String instancedVert = vertSrc.substr(0, newline + 1)
            + "#define INSTANCED 1\n"
            + vertSrc.substr(newline + 1);
        const String depthFrag = "#version 330 core\nvoid main() {}\n";
        return GetGraphicsDevice().CreateShader(instancedVert, depthFrag);
    }

    SharedPtr<Shader> RenderModule::GetOrCreateDepthPassShader(const SharedPtr<Shader> &shader)
    {
        auto it = m_depthPassShaderCache.find(shader.get());
        return it != m_depthPassShaderCache.end() ? it->second : nullptr;
    }

    SharedPtr<Shader> RenderModule::GetOrCreateDepthPassInstancedShader(const SharedPtr<Shader> &shader)
    {
        auto it = m_depthPassInstancedShaderCache.find(shader.get());
        return it != m_depthPassInstancedShaderCache.end() ? it->second : nullptr;
    }

    void RenderModule::SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Material>& material, const Vector<Matrix4>& transforms)
    {
        uint32 byteSize = static_cast<uint32>(transforms.size() * sizeof(Matrix4));
        auto cacheKey = std::make_pair(buffers.vb.get(), material.get());
        auto it = m_instanceMaterialBufferCache.find(cacheKey);
        if (it == m_instanceMaterialBufferCache.end())
        {
            auto instanceBuffer = m_graphicsDevice->CreateDynamicVertexBuffer(byteSize);
                instanceBuffer->SetLayout({
                { ShaderDataType::Float4, "a_InstanceTransform0" },
                { ShaderDataType::Float4, "a_InstanceTransform1" },
                { ShaderDataType::Float4, "a_InstanceTransform2" },
                { ShaderDataType::Float4, "a_InstanceTransform3" },
            });
            m_instanceMaterialBufferCache[cacheKey] = instanceBuffer;
            it = m_instanceMaterialBufferCache.find(cacheKey);
        }

        SharedPtr<VertexBuffer>& instanceBuffer = it->second;
        instanceBuffer->SetData(transforms.data(), byteSize);

        material->Bind();  // binds regular shader, uploads uniforms + textures

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
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderModule::SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Shader>& instancedShader, const Vector<Matrix4>& transforms)
    {
        uint32 byteSize = static_cast<uint32>(transforms.size() * sizeof(Matrix4));
        auto cacheKey = std::make_pair(buffers.vb.get(), instancedShader.get());
        auto it = m_instanceShaderBufferCache.find(cacheKey);
        if (it == m_instanceShaderBufferCache.end())
        {
            auto instanceBuffer = m_graphicsDevice->CreateDynamicVertexBuffer(byteSize);
            instanceBuffer->SetLayout({
                { ShaderDataType::Float4, "a_InstanceTransform0" },
                { ShaderDataType::Float4, "a_InstanceTransform1" },
                { ShaderDataType::Float4, "a_InstanceTransform2" },
                { ShaderDataType::Float4, "a_InstanceTransform3" },
            });
            m_instanceShaderBufferCache[cacheKey] = instanceBuffer;
            it = m_instanceShaderBufferCache.find(cacheKey);
        }
        SharedPtr<VertexBuffer>& instanceBuffer = it->second;
        instanceBuffer->SetData(transforms.data(), byteSize);
        instancedShader->Bind();
        instancedShader->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderModule::SubmitBatched(const SharedPtr<Material>& material, const Vector<RenderCommand>& commands)
    {
        Vector<Vertex> vertices;
        Vector<uint32> indices;

        for (const auto& cmd : commands)
        {
            uint32 base = static_cast<uint32>(vertices.size());
            for (const Vertex& v : cmd.mesh->GetVertices())
            {
                Vertex tv = v;
                const Matrix4& t = cmd.transform;
                tv.m_position = Vec3(
                    t(0,0) * v.m_position.x + t(0,1) * v.m_position.y + t(0,2) * v.m_position.z + t(0,3),
                    t(1,0) * v.m_position.x + t(1,1) * v.m_position.y + t(1,2) * v.m_position.z + t(1,3),
                    t(2,0) * v.m_position.x + t(2,1) * v.m_position.y + t(2,2) * v.m_position.z + t(2,3)
                );
                vertices.push_back(tv);
            }
            for (uint32 idx : cmd.mesh->GetIndices())
            {
                indices.push_back(base + idx);
            }
        }

        uint32 vbSize  = static_cast<uint32>(vertices.size() * sizeof(Vertex));
        uint32 ibCount = static_cast<uint32>(indices.size());
        Material* key  = material.get();

        if (!m_batchVBCache.count(key))
        {
            auto vb = m_graphicsDevice->CreateDynamicVertexBuffer(vbSize);
            vb->SetLayout(Vertex::GetLayout());
            m_batchVBCache[key] = vb;
        }

        if (!m_batchIBCache.count(key))
        {
            m_batchIBCache[key] = m_graphicsDevice->CreateDynamicIndexBuffer(ibCount);
        }

        m_batchVBCache[key]->SetData(vertices.data(), vbSize);
        m_batchIBCache[key]->SetData(indices.data(), ibCount);

        material->Bind();
        material->GetShader()->UploadUniform("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        material->GetShader()->UploadUniform("u_Transform", Matrix4::Identity());
        
        m_graphicsDevice->DrawIndexed(m_batchVBCache[key], m_batchIBCache[key]);
        m_batchCallCount++;
        m_batchObjectCount += commands.size();
    }

    void RenderModule::ResetDebugDrawCounters()
    {
        m_instanceCallCount = 0;
        m_instanceObjectCount = 0;
        m_batchCallCount = 0;
        m_batchObjectCount = 0;
    }

    void RenderModule::RenderToScreen()
    {
        m_frameBuffer->Unbind();
        m_graphicsDevice->SetDepthTestEnabled(false);
        m_graphicsDevice->SetClearColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_graphicsDevice->Clear();
        m_screenQuadShader->Bind();
        m_screenQuadShader->UploadUniform("u_screenTexture", 0);
        m_graphicsDevice->BindFrameBufferTexture(*m_frameBuffer, 0);
        m_graphicsDevice->DrawIndexed(m_screenQuadVB, m_screenQuadIB);
        m_graphicsDevice->SetDepthTestEnabled(true);
    }

    GraphicsDevice::API RenderModule::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }
}
