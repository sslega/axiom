#include "RenderSubsystem.h"
#include "Core/Application.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Resources/MaterialResource.h"
#include "Resources/ResourceSubsystem.h"
#include "Scene/WorldSubsystem.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/MeshComponent.h"
#include "Renderer/LightComponent.h"
#include "Scene/TransformComponent.h"
#include "Core/Log.h"
#include "Core/Types.h"
#include "Resources/MaterialResource.h"
#include "FrameBuffer.h"
#include <imgui.h>
#include "Geometry/Quad.h"
#include <algorithm>

namespace axiom
{
    RenderSubsystem::RenderSubsystem(Application& application)
    : ApplicationSubsystem(application)
    {
    }

    void RenderSubsystem::OnInitialize()
    {
        auto API = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();
        m_graphicsDevice = GraphicsDevice::Create(API, window);

        FramebufferSpec fbSpec;
        fbSpec.width = window.GetWidth();
        fbSpec.height = window.GetHeight();
        m_frameBuffer = m_graphicsDevice->CreateFrameBuffer(fbSpec);

        FramebufferSpec fbShadowSpec;
        fbShadowSpec.width = 1024;
        fbShadowSpec.height = 1024;
        fbShadowSpec.depthOnly = true;
        m_shadowMapFrameBuffer = m_graphicsDevice->CreateFrameBuffer(fbShadowSpec);

        Quad quad;
        m_screenQuadVB = m_graphicsDevice->CreateVertexBuffer(quad);
        m_screenQuadIB = m_graphicsDevice->CreateIndexBuffer(quad);
        m_screenQuadShader = GetShader("engine://Shaders/FullScreen.glsl");
        m_debugDrawMaterial = GetMaterial("engine://Shaders/DebugDraw.glsl");
    }

    void RenderSubsystem::OnShutdown()
    {
    }

    void RenderSubsystem::OnUpdate(float deltaTime)
    {
    }

    void RenderSubsystem::OnBeginFrame()
    {
        ResetDebugDrawCounters();
        BeginScene();
    }

    void RenderSubsystem::OnRender()
    {    
        auto now = std::chrono::steady_clock::now();
        m_dt = std::chrono::duration<float>(now - m_lastRenderTime).count();
        m_lastRenderTime = now;

        
        WorldSubsystem& worldSubsystem = GetSubsystem<WorldSubsystem>();

        Scene& scene = worldSubsystem.GetActiveScene();
        auto meshComponents = scene.GetComponents<MeshComponent>();

        Vector<RenderCommand> renderCommands;
        for(MeshComponent* meshComponent : meshComponents)
        {
            if (!meshComponent->IsVisible()) continue;
            if (!meshComponent->GetMaterial() || !meshComponent->GetMesh()) continue;
            TransformComponent* tc = meshComponent->GetEntity().GetComponent<TransformComponent>();
            renderCommands.push_back({ meshComponent->GetMesh(), meshComponent->GetMaterial(), tc ? tc->GetTransform() : Matrix4::Identity() });
        }

        Vector<View> views = BuildViews(scene);
        for(auto& view : views)
        {
            ExecuteView(view, renderCommands);
        }

        OnGUI();
        RenderToScreen();
    }

    void RenderSubsystem::OnEndFrame()
    {
        EndScene(); 
    }

    GraphicsDevice& RenderSubsystem::GetGraphicsDevice() const
    {
        return *m_graphicsDevice;
    }

    void RenderSubsystem::BeginScene()
    {
        WorldSubsystem& worldSubsystem = GetSubsystem<WorldSubsystem>();

        Scene& scene = worldSubsystem.GetActiveScene();

        Vector<CameraComponent*> cameras = scene.GetComponents<CameraComponent>();
        AX_ASSERT(!cameras.empty(), "No active Camera!");
        CameraComponent* cameraComponent = cameras[0];
        TransformComponent* cameraTransform = cameraComponent->GetEntity().GetComponent<TransformComponent>();
        cameraComponent->SetAspectRatio(GetApp().GetApplicationWindow().GetAspectRatio());
        Matrix4 viewMatrix = cameraTransform ? cameraTransform->GetViewMatrix() : Matrix4::Identity();
        m_renderSceneData.viewProjectionMatrix = cameraComponent->GetProjectionMatrix() * viewMatrix;
        m_renderSceneData.cameraPosition = cameraTransform->position;
        
        Vector<DirectionalLightComponent*> directionalLight = scene.GetComponents<DirectionalLightComponent>();
        if(directionalLight.size() > 1)
        {
            Log::Error("More than one DirectionalLightComponent present!");
        }

        if(!directionalLight.empty())
        {
            DirectionalLightComponent* light = directionalLight[0];
            TransformComponent* lightTransform = light->GetEntity().GetComponent<TransformComponent>();
            AX_ASSERT(lightTransform, "No TransformComponent present with DirectionalLightComponent!");
            m_renderSceneData.hasDirectionalLight = true;
            m_renderSceneData.lightColor = light->color * light->intensity;
            m_renderSceneData.lightDirection = lightTransform->Forward();
            Vec3 eye = -m_renderSceneData.lightDirection;
            Vec3 center = Vec3(0,0,0);
            Matrix4 lightViewMatrix = Matrix4::LookAt(eye, center, Vec3(0, 1, 0));

            Vector<Vec3> frustumCorners = GetFrustumCornersWorldSpace(m_renderSceneData.viewProjectionMatrix.Inverse());
            float minX = FLT_MAX;
            float minY = FLT_MAX;
            float minZ = FLT_MAX;
            float maxX = -FLT_MAX;
            float maxY = -FLT_MAX;
            float maxZ = -FLT_MAX;
            for(Vec3& frustumCorner : frustumCorners)
            {
                Vec3 lightSpaceCorner  = lightViewMatrix.TransformPoint(frustumCorner);
                minX = std::min(minX, lightSpaceCorner.x);
                maxX = std::max(maxX, lightSpaceCorner.x);
                minY = std::min(minY, lightSpaceCorner.y);
                maxY = std::max(maxY, lightSpaceCorner.y);
                minZ = std::min(minZ, lightSpaceCorner.z);
                maxZ = std::max(maxZ, lightSpaceCorner.z);
            }

            Matrix4 lightProjectionMatrix = Matrix4::Ortho(minX, maxX, minY, maxY, -minZ, -maxZ);

            m_renderSceneData.lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;
        }
        else
        {
            m_renderSceneData.hasDirectionalLight = false;
            m_renderSceneData.lightColor = Vec3(0);
            m_renderSceneData.lightDirection = Vec3(1,0,0);
            
            m_renderSceneData.lightViewProjectionMatrix = Matrix4::Identity();
        }
    }

    void RenderSubsystem::EndScene()
    {
        m_graphicsDevice->SwapBuffers();
    }

    void RenderSubsystem::OnGUI()
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
        
        if(m_renderSceneData.hasDirectionalLight)
        {
            uint32 depthId = m_shadowMapFrameBuffer->GetDepthAttachmentID();
            ImGui::Begin("ShadowMap");
            ImGui::Image((ImTextureID)(uintptr_t)depthId, ImVec2(256, 256), ImVec2(0,1), ImVec2(1,0));
            ImGui::End();
        }
    }

    void RenderSubsystem::Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<MaterialResource>& material, const Matrix4& transform)
    {

        auto& m = m_debugDrawMode > 0 ? m_debugDrawMaterial : material;
        m->SetUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        m->SetUniform("u_LocalToWorld", transform);
        m->SetUniform("u_WorldToLocal", transform.Inverse());
        m->SetUniform("u_DebugMode", m_debugDrawMode);
        Vector<String> defines;
        if(m_renderSceneData.hasDirectionalLight)
        {
            m->SetUniform("u_LightDir", m_renderSceneData.lightDirection);
            m->SetUniform("u_LightColor", m_renderSceneData.lightColor);
            m->SetUniform("u_CameraPos", m_renderSceneData.cameraPosition);
            m->SetUniform("u_LightViewProjection", m_renderSceneData.lightViewProjectionMatrix);
            m->SetUniform("u_ShadowMap", 1);  // texture slot 1
            m_graphicsDevice->BindFrameBufferTexture(*m_shadowMapFrameBuffer, 1);
            defines.push_back("HAS_DIRECTIONAL_LIGHT");
        }

        m->Bind(defines);
        m_graphicsDevice->DrawIndexed(vb, ib);
        m_callCount++;
    }

    void RenderSubsystem::Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform)
    {
        shader->Bind();
        shader->UploadUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        shader->UploadUniform("u_LocalToWorld", transform);
        m_graphicsDevice->DrawIndexed(vb, ib);
        m_callCount++;
}

    SharedPtr<MaterialResource> RenderSubsystem::GetMaterial(const String path)
    {
        return MakeShared<MaterialResource>(GetShader(path));
    }

    SharedPtr<Shader> RenderSubsystem::GetShader(const String path)
    {
        auto it = m_shaderCache.find(path);
        if(it == m_shaderCache.end())
        {
            auto shader = CreateShader(path);
            m_shaderCache[path] = shader;
            it = m_shaderCache.find(path);

            auto depthShader = CreateDepthPassShader(path);
            m_depthPassShaderCache[shader.get()] = depthShader;

            auto depthInstancedShader = CreateDepthPassInstancedShader(path);
            m_depthPassInstancedShaderCache[shader.get()] = depthInstancedShader;
        }
        return it->second;
    }

    SharedPtr<Shader> RenderSubsystem::CreateShader(const String path)
    {
        auto shaderResource = GetSubsystem<ResourceSubsystem>().Load<ShaderResource>(path);
        return GetGraphicsDevice().CreateShader(*shaderResource);
    }

    RenderSubsystem::MeshBuffers RenderSubsystem::GetOrCreateBuffers(const SharedPtr<MeshResource> &mesh)
    {
        auto it = m_meshCache.find(mesh.get());
        if (it != m_meshCache.end()) return it->second;

        MeshBuffers buffers;
        buffers.vb = m_graphicsDevice->CreateVertexBuffer(*mesh);
        buffers.ib = m_graphicsDevice->CreateIndexBuffer(*mesh);
        m_meshCache[mesh.get()] = buffers;
        return buffers;
    }

    SharedPtr<Shader> RenderSubsystem::CreateDepthPassShader(const String path)
    {
        auto shaderResource = GetSubsystem<ResourceSubsystem>().Load<ShaderResource>(path);
        const String depthFrag = "#version 330 core\nvoid main() {}\n";
        return GetGraphicsDevice().CreateShader(shaderResource->GetVertexSource(), depthFrag);
    }

    SharedPtr<Shader> RenderSubsystem::CreateDepthPassInstancedShader(const String path)
    {
        auto shaderResource = GetSubsystem<ResourceSubsystem>().Load<ShaderResource>(path);
        const String& vertSrc = shaderResource->GetVertexSource();
        size_t newline = vertSrc.find('\n', vertSrc.find("#version"));
        String instancedVert = vertSrc.substr(0, newline + 1)
            + "#define INSTANCED 1\n"
            + vertSrc.substr(newline + 1);
        const String depthFrag = "#version 330 core\nvoid main() {}\n";
        return GetGraphicsDevice().CreateShader(instancedVert, depthFrag);
    }

    SharedPtr<Shader> RenderSubsystem::GetOrCreateDepthPassShader(const SharedPtr<Shader> &shader)
    {
        auto it = m_depthPassShaderCache.find(shader.get());
        return it != m_depthPassShaderCache.end() ? it->second : nullptr;
    }

    SharedPtr<Shader> RenderSubsystem::GetOrCreateDepthPassInstancedShader(const SharedPtr<Shader> &shader)
    {
        auto it = m_depthPassInstancedShaderCache.find(shader.get());
        return it != m_depthPassInstancedShaderCache.end() ? it->second : nullptr;
    }

    void RenderSubsystem::SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<MaterialResource>& material, const Vector<Matrix4>& transforms)
    {
        auto& m = m_debugDrawMode > 0 ? m_debugDrawMaterial : material;

        Vector<Matrix4> instanceData;
        instanceData.reserve(transforms.size() * 2);
        for (const auto& t : transforms)
        {
            instanceData.push_back(t);
            instanceData.push_back(t.Inverse());
        }
        uint32 byteSize = static_cast<uint32>(instanceData.size() * sizeof(Matrix4));

        auto cacheKey = std::make_pair(buffers.vb.get(), m.get());
        auto it = m_instanceMaterialBufferCache.find(cacheKey);
        if (it == m_instanceMaterialBufferCache.end())
        {
            auto instanceBuffer = m_graphicsDevice->CreateDynamicVertexBuffer(byteSize);
            instanceBuffer->SetLayout({
                { ShaderDataType::Float4, "a_InstanceTransform0" },
                { ShaderDataType::Float4, "a_InstanceTransform1" },
                { ShaderDataType::Float4, "a_InstanceTransform2" },
                { ShaderDataType::Float4, "a_InstanceTransform3" },
                { ShaderDataType::Float4, "a_InstanceInverse0" },
                { ShaderDataType::Float4, "a_InstanceInverse1" },
                { ShaderDataType::Float4, "a_InstanceInverse2" },
                { ShaderDataType::Float4, "a_InstanceInverse3" },
            });
            m_instanceMaterialBufferCache[cacheKey] = instanceBuffer;
            it = m_instanceMaterialBufferCache.find(cacheKey);
        }

        SharedPtr<VertexBuffer>& instanceBuffer = it->second;
        instanceBuffer->SetData(instanceData.data(), byteSize);

        // Bind the INSTANCED variant — uploads all material uniforms to the correct GL program
        m->SetUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        m->Bind({"INSTANCED"});
        // material->GetShader()->GetVariant({"INSTANCED"})->UploadUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
        m_callCount++;
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderSubsystem::SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Shader>& instancedShader, const Vector<Matrix4>& transforms)
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
        instancedShader->UploadUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
        m_callCount++;
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderSubsystem::SubmitBatched(const SharedPtr<MaterialResource>& material, const Vector<RenderCommand>& commands)
    {
        Vector<Vertex> vertices;
        Vector<uint32> indices;
        auto& m = m_debugDrawMode > 0 ? m_debugDrawMaterial : material;

        for (const auto& cmd : commands)
        {
            uint32 base = static_cast<uint32>(vertices.size());
            Matrix4 localToWorld = cmd.transform;
            for (const Vertex& v : cmd.mesh->GetVertices())
            {
                Vertex tv = v;
                const Matrix4& t = cmd.transform;
                tv.m_position = localToWorld.TransformPoint(v.m_position);
                tv.m_normal   = Normalize(localToWorld.TransformDirection(v.m_normal));
                vertices.push_back(tv);
            }
            for (uint32 idx : cmd.mesh->GetIndices())
            {
                indices.push_back(base + idx);
            }
        }

        uint32 vbSize  = static_cast<uint32>(vertices.size() * sizeof(Vertex));
        uint32 ibCount = static_cast<uint32>(indices.size());
        MaterialResource* key  = m.get();

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

        auto& vb = m_batchVBCache[key];
        auto& ib = m_batchIBCache[key];

        Submit(vb, ib, m, Matrix4::Identity());

        // m->SetUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        // m->Bind();
        // m->GetShader()->UploadUniform("u_ViewProjection", m_renderSceneData.viewProjectionMatrix);
        // m->GetShader()->UploadUniform("u_LocalToWorld", Matrix4::Identity());
        
        // m_graphicsDevice->DrawIndexed(m_batchVBCache[key], m_batchIBCache[key]);
        // m_callCount++;
        m_batchCallCount++;
        m_batchObjectCount += commands.size();
    }

    void RenderSubsystem::ResetDebugDrawCounters()
    {
        m_callCount = 0;
        m_instanceCallCount = 0;
        m_instanceObjectCount = 0;
        m_batchCallCount = 0;
        m_batchObjectCount = 0;
    }

    void RenderSubsystem::RenderToScreen()
    {
        m_frameBuffer->Unbind();
        m_graphicsDevice->SetDepthTestEnabled(false);
        m_graphicsDevice->SetClearColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        m_graphicsDevice->Clear();
        m_screenQuadShader->Bind();
        m_screenQuadShader->UploadUniform("u_ScreenTexture", 0);
        m_graphicsDevice->BindFrameBufferTexture(*m_frameBuffer, 0);
        m_graphicsDevice->DrawIndexed(m_screenQuadVB, m_screenQuadIB);
        m_graphicsDevice->SetDepthTestEnabled(true);
    }

    void RenderSubsystem::RenderShadowPass(const Matrix4& viewProjectionMatrix, const Vector<RenderCommand>& commands)
    {
        //TODO: Add batching/instancing
        for (const auto& cmd : commands)
        {
            auto buffers = GetOrCreateBuffers(cmd.mesh);
            //TODO: wrap it around Submit later on 
            auto shader = GetOrCreateDepthPassShader(cmd.material->GetShader());
            shader->Bind();
            shader->UploadUniform("u_ViewProjection", viewProjectionMatrix);
            shader->UploadUniform("u_LocalToWorld", cmd.transform);
            m_graphicsDevice->DrawIndexed(buffers.vb, buffers.ib);
            m_callCount++;
        }
    }

    void RenderSubsystem::RenderScenePass(const Matrix4& viewProjectionMatrix, const Vector<RenderCommand>& commands)
    {
        // 2. Depth Pre-Pass
        m_graphicsDevice->SetColorWriteEnabled(false);
        m_graphicsDevice->SetDepthWriteEnabled(true);

        UnorderedMap<MeshResource*, Vector<Matrix4>> depthGroups;
        for (const auto& cmd : commands)
            depthGroups[cmd.mesh.get()].push_back(cmd.transform);

        for (const auto& [meshPtr, transforms] : depthGroups)
        {
            auto it = std::find_if(commands.begin(), commands.end(),
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

        //TODO: Clean up instancing/batching toggle flags
        // 3. Group by (mesh, material)
        PairMap<InstanceGroupKey, Vector<Matrix4>> instanceGroups;
        for (auto& renderCommand : commands)
        {
            InstanceGroupKey groupKey{ renderCommand.mesh.get(), renderCommand.material.get() };
            instanceGroups[groupKey].push_back(renderCommand.transform);
        }
        
        // 4. Dispatch Intanced meshesh, collect batch candidates
        UnorderedMap<MaterialResource*, Vector<RenderCommand>> batchCandidates;

        for (auto& [key, transforms] : instanceGroups)
        {
            auto it = std::find_if(commands.begin(), commands.end(), [&](const RenderCommand& c){
                return c.mesh.get() == key.first && c.material.get() == key.second;
            });
            auto buffers = GetOrCreateBuffers(it->mesh);
            if (transforms.size() > 1 && m_instancingEnabled)
            {
                SubmitInstanced(buffers, it->material, transforms);
            }
            else
            {
                for(auto& t : transforms)
                {
                    batchCandidates[key.second].push_back({ it->mesh, it->material, t });
                }
            }
        }

        // 5. Draw batch and everything else
        for (auto& [matPtr, cmds] : batchCandidates)
        {
            if (cmds.size() > 1 && m_batchingEnabled)
                SubmitBatched(cmds[0].material, cmds);
            else
            {
                for(auto& c : cmds)
                {
                    auto buffers = GetOrCreateBuffers(c.mesh);
                    Submit(buffers.vb, buffers.ib, c.material, c.transform);
                }
            }
        }
    }

    Vector<Vec3> RenderSubsystem::GetFrustumCornersWorldSpace(const Matrix4& invViewProj)
    {
        Vector<Vec3> corners = {
            {-1, -1, -1}, {1, -1, -1},
            {-1,  1, -1}, {1,  1, -1},
            {-1, -1,  1}, {1, -1,  1},
            {-1,  1,  1}, {1,  1,  1}
        };
        for(int i = 0; i < corners.size(); ++i)
        {
            corners[i] = invViewProj.TransformPoint(corners[i]);
        }
        return corners;
    }

    Vector<View> RenderSubsystem::BuildViews(Scene &scene)
    {
        Vector<View> views;
        // Directional Shadowmap
        if(m_renderSceneData.hasDirectionalLight)
        {
            View view = View();
            view.debugName = "ShadowPass";
            view.passType = PassType::DepthOnly;
            view.renderTarget = m_shadowMapFrameBuffer;
            view.viewProjection = m_renderSceneData.lightViewProjectionMatrix;
            views.push_back(view);
        }
        // SceneColor
        {
            View view = View();
            view.debugName = "SceneColor";
            view.passType = PassType::Full;
            view.renderTarget = m_frameBuffer;
            view.viewProjection = m_renderSceneData.viewProjectionMatrix;
            views.push_back(view);
        }

        return views;
    }

    void RenderSubsystem::ExecuteView(const View& view, const Vector<RenderCommand>& commands)
    {
        if(!view.renderTarget) return;
        
        m_graphicsDevice->SetColorWriteEnabled(true);
        m_graphicsDevice->SetDepthWriteEnabled(true);
        m_graphicsDevice->SetDepthFunction(DepthFunction::Less);
        view.renderTarget->Bind();
        m_graphicsDevice->SetViewport(0, 0, view.renderTarget->GetWidth(), view.renderTarget->GetHeight());
        m_graphicsDevice->Clear();

        switch (view.passType)
        {
        case PassType::DepthOnly:
            RenderShadowPass(view.viewProjection, commands);
            break;
        case PassType::Full:
            RenderScenePass(view.viewProjection, commands);
            break;
        }
    }

    GraphicsDevice::API RenderSubsystem::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }
}
