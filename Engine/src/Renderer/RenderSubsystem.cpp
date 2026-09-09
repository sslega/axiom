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
#include "Renderer/FrameBuffer.h"
#include "Scene/TransformComponent.h"
#include "Core/Log.h"
#include "Core/Types.h"
#include "Resources/MaterialResource.h"
#include <imgui.h>
#include "Geometry/Quad.h"
#include <algorithm>

namespace axiom
{
    RenderSubsystem::RenderSubsystem(Application& application)
    : ApplicationSubsystem(application)
    {
    }

    void RenderSubsystem::ReloadShaders()
    {
        Log::Info("Reloading shaders...");
        auto& resourceSubsystem = GetSubsystem<ResourceSubsystem>();
        // path is const String&, shader is SharedPtr<Shader>&
        for (auto& [path, shader] : m_shaderCache)
        {
            resourceSubsystem.Evict(path);
            auto shaderResource = resourceSubsystem.Load<ShaderResource>(path);
            bool result = shader->Reload(shaderResource->GetVertexSource(), shaderResource->GetFragmentSource(), shaderResource->GetSourceMap());
            if (!result)
            {
                Log::Error("Failed to reload shader: {}", path);
            }
        }
        Log::Info("Reloading shaders... Done!");
    }

    void RenderSubsystem::OnInitialize()
    {
        auto API = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();
        m_graphicsDevice = GraphicsDevice::Create(API, window);

        // FramebufferSpec fbSpec;
        // fbSpec.width = window.GetWidth();
        // fbSpec.height = window.GetHeight();
        // m_frameBuffer = m_graphicsDevice->CreateFrameBuffer(fbSpec);

        FramebufferSpec fbShadowSpec;
        fbShadowSpec.width = 2048;
        fbShadowSpec.height = 2048;
        fbShadowSpec.depthOnly = true;
        m_shadowMapFrameBuffer = m_graphicsDevice->CreateFrameBuffer(fbShadowSpec);

        Quad quad;
        m_screenQuadVB = m_graphicsDevice->CreateVertexBuffer(quad);
        m_screenQuadIB = m_graphicsDevice->CreateIndexBuffer(quad);
        m_screenQuadShader = GetShader("engine://Shaders/FullScreen.glsl");
        m_debugDrawMaterial = GetMaterial("engine://Shaders/DebugDraw.glsl");
        const char* vertSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 a_Position;
        uniform mat4 u_ViewProjection;
        uniform mat4 u_LocalToWorld;
        void main()
        {
            gl_Position = u_ViewProjection * u_LocalToWorld * vec4(a_Position, 1.0);
        }
        )";
        const char* fragSrc = R"(
        #version 330 core
        uniform float u_time;
        layout(location = 0) out vec4 color;
        void main()
        {
            float t = abs(sin(u_time * 3.0));
            //color = vec4(t, 0.0, t, 1.0);
            color = vec4(t,0,1,1.0);
        }
        )";

        m_errorShader = GetGraphicsDevice().CreateShader(vertSrc, fragSrc, {});
        m_errorMaterial = MakeShared<MaterialResource>(m_errorShader);

        m_lastRenderTime = std::chrono::steady_clock::now();
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
        m_elapsedTime += m_dt;
        m_lastRenderTime = now;
        
        WorldSubsystem& worldSubsystem = GetSubsystem<WorldSubsystem>();

        Scene& scene = worldSubsystem.GetActiveScene();
        // Directional Light
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
            Vec3 eye = m_renderSceneData.lightDirection;
            m_renderSceneData.lightViewMatrix = Matrix4::LookAt(eye, Vec3(0,0,0), Vec3(0,1,0));
        }
        else
        {
            m_renderSceneData.hasDirectionalLight = false;
            m_renderSceneData.lightColor = Vec3(0);
            m_renderSceneData.lightDirection = Vec3(1,0,0);
            
            m_renderSceneData.lightViewMatrix = Matrix4::Identity();
        }


        // Mesh Components
        auto meshComponents = scene.GetComponents<MeshComponent>();

        Vector<RenderCommand> renderCommands;
        for(MeshComponent* meshComponent : meshComponents)
        {
            if (!meshComponent->IsVisible()) continue;
            if (!meshComponent->material || !meshComponent->mesh) continue;
            TransformComponent* tc = meshComponent->GetEntity().GetComponent<TransformComponent>();
            renderCommands.push_back({ meshComponent->mesh.GetResource(), meshComponent->material.GetResource(), tc ? tc->GetTransform() : Matrix4::Identity() });
        }

        // View Pass
        for(const auto& view : m_views)
        {
            ExecuteView(view, renderCommands);
        }

        m_graphicsDevice->BindDefaultFrameBuffer();
        m_graphicsDevice->SetClearColor(Vec4(0,0,0,1));
        m_graphicsDevice->Clear();

        OnGUI();
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
        m_views.clear();
        
        m_renderSceneData.time = m_elapsedTime;
    }

    void RenderSubsystem::EndScene()
    {
        m_graphicsDevice->SwapBuffers();
    }

    void RenderSubsystem::OnGUI()
    {
        return;
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
            ImGui::Image((ImTextureID)(uintptr_t)depthId, ImVec2(512, 512), ImVec2(0,1), ImVec2(1,0));
            ImGui::End();
        }
    }

    void RenderSubsystem::Submit(const RenderViewData& viewData, const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<MaterialResource>& material, const Matrix4& transform)
    {
        const auto& effective = material->IsValid() ? material : m_errorMaterial;
        auto& m = m_debugDrawMode > 0 ? m_debugDrawMaterial : effective;
        m->SetUniform("u_ViewProjection", viewData.viewProjectionMatrix);
        m->SetUniform("u_LocalToWorld", transform);
        m->SetUniform("u_WorldToLocal", transform.Inverse());
        m->SetUniform("u_DebugMode", m_debugDrawMode);

        m->SetUniform("u_time", m_renderSceneData.time);

        Vector<String> defines;
        if(m_renderSceneData.hasDirectionalLight)
        {
            m->SetUniform("u_LightDir", m_renderSceneData.lightDirection);
            m->SetUniform("u_LightColor", m_renderSceneData.lightColor);
            m->SetUniform("u_CameraPos", viewData.cameraPosition);
            m->SetUniform("u_LightViewProjection", viewData.lightViewProjectionMatrix);
            m->SetUniform("u_ShadowMap", 1);  // texture slot 1
            m_graphicsDevice->BindFrameBufferTexture(*m_shadowMapFrameBuffer, 1);
            defines.push_back("HAS_DIRECTIONAL_LIGHT");
        }

        m->Bind(defines);

        m_graphicsDevice->DrawIndexed(vb, ib);
        m_callCount++;
    }

    void RenderSubsystem::Submit(const RenderViewData& viewData, const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform)
    {
        shader->Bind();
        shader->UploadUniform("u_ViewProjection", viewData.viewProjectionMatrix);
        shader->UploadUniform("u_LocalToWorld", transform);
        m_graphicsDevice->DrawIndexed(vb, ib);
        m_callCount++;
    }

    void RenderSubsystem::SubmitView(const View &view)
    {
        m_views.push_back(view);
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
        }
        return it->second;
    }

    SharedPtr<Shader> RenderSubsystem::CreateShader(const String path)
    {
        auto shaderResource = GetSubsystem<ResourceSubsystem>().Load<ShaderResource>(path);
        auto shader = GetGraphicsDevice().CreateShader(*shaderResource);
        if (!shader) return m_errorShader;
        return shader;
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

    void RenderSubsystem::SubmitInstanced(const RenderViewData& viewData, const MeshBuffers& buffers, const SharedPtr<MaterialResource>& material, const Vector<Matrix4>& transforms)
    {
        const auto& effective = material->IsValid() ? material : m_errorMaterial;
        auto& m = m_debugDrawMode > 0 ? m_debugDrawMaterial : effective;

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
        m->SetUniform("u_ViewProjection", viewData.viewProjectionMatrix);
        m->Bind({"INSTANCED"});

        
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
        m_callCount++;
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderSubsystem::SubmitInstanced(const RenderViewData& viewData, const MeshBuffers& buffers, const SharedPtr<Shader>& instancedShader, const Vector<Matrix4>& transforms)
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
        instancedShader->UploadUniform("u_ViewProjection", viewData.viewProjectionMatrix);
        m_graphicsDevice->DrawIndexedInstanced(buffers.vb, buffers.ib, instanceBuffer, static_cast<uint32>(transforms.size()));
        m_callCount++;
        m_instanceCallCount++;
        m_instanceObjectCount += transforms.size();
    }

    void RenderSubsystem::SubmitBatched(const RenderViewData& viewData, const SharedPtr<MaterialResource>& material, const Vector<RenderCommand>& commands)
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

        Submit(viewData, vb, ib, m, Matrix4::Identity());

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

    void RenderSubsystem::RenderShadowPass(const Matrix4& viewProjectionMatrix, const Vector<RenderCommand>& commands)
    {
        //TODO: Add batching/instancing
        for (const auto& cmd : commands)
        {
            auto buffers = GetOrCreateBuffers(cmd.mesh);
            //TODO: wrap it around Submit later on 
            auto shader = cmd.material->GetShader();            
            auto variant = shader->GetVariant({"DEPTH_PASS"});

            variant->Bind();
            variant->UploadUniform("u_ViewProjection", viewProjectionMatrix);
            variant->UploadUniform("u_LocalToWorld", cmd.transform);
            m_graphicsDevice->DrawIndexed(buffers.vb, buffers.ib);
            m_callCount++;
        }
    }

    void RenderSubsystem::RenderShadowMap(const Matrix4& lightViewProjection, const Vector<RenderCommand>& commands)
    {
         m_graphicsDevice->SetColorWriteEnabled(true);
        m_graphicsDevice->SetDepthWriteEnabled(true);
        m_graphicsDevice->SetDepthFunction(DepthFunction::Less);
        m_shadowMapFrameBuffer->Bind();
        m_graphicsDevice->SetViewport(0, 0, m_shadowMapFrameBuffer->GetWidth(), m_shadowMapFrameBuffer->GetHeight());
        m_graphicsDevice->Clear();
        RenderShadowPass(lightViewProjection, commands);
    }

    void RenderSubsystem::RenderScenePass(const RenderSceneData& sceneData, const RenderViewData& viewData, const Vector<RenderCommand>& commands)
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
            auto shader = it->material->GetShader();

            if (transforms.size() > 1)
            {
                auto variant = shader->GetVariant({"DEPTH_PASS", "INSTANCED"});
                SubmitInstanced(viewData, buffers, variant, transforms);
            }
            else
            {
                auto variant = shader->GetVariant({"DEPTH_PASS"});
                Submit(viewData, buffers.vb, buffers.ib, variant, transforms[0]);
            }

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
                SubmitInstanced(viewData, buffers, it->material, transforms);
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
                SubmitBatched(viewData, cmds[0].material, cmds);
            else
            {
                for(auto& c : cmds)
                {
                    auto buffers = GetOrCreateBuffers(c.mesh);
                    Submit(viewData, buffers.vb, buffers.ib, c.material, c.transform);
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

    Matrix4 RenderSubsystem::ComputeShadowProjection(const Matrix4& lightViewMatrix, const Matrix4& cameraViewMatrix)
    {
        Vector<Vec3> frustumCorners = GetFrustumCornersWorldSpace(cameraViewMatrix.Inverse());
        Vec3 frustumCenter = 0;
        float frustumRadius = 0;
        float minZ = FLT_MAX;
        float maxZ = -FLT_MAX;
        for(Vec3& frustumCorner : frustumCorners)
        {
            Vec3 lightSpaceCorner  = lightViewMatrix.TransformPoint(frustumCorner);
            minZ = std::min(minZ, lightSpaceCorner.z);
            maxZ = std::max(maxZ, lightSpaceCorner.z);
            frustumCenter += frustumCorner;
        }
        frustumCenter /= frustumCorners.size();
        Vec3 lightSpaceCenter = lightViewMatrix.TransformPoint(frustumCenter);

        for(Vec3& frustumCorner : frustumCorners)
        {
            // This can be in world space, distance is identical in light space and world space
            frustumRadius = std::max(frustumRadius, Length(frustumCorner - frustumCenter));
        }

        float texelSize = (2 * frustumRadius) / m_shadowMapFrameBuffer->GetWidth();
        lightSpaceCenter = Vec3(
            std::round(lightSpaceCenter.x / texelSize) * texelSize,
            std::round(lightSpaceCenter.y / texelSize) * texelSize,
            lightSpaceCenter.z
        );

        float nearPullback = 10.0f;
        float farPullback  = 10.0f;

        Matrix4 lightProjectionMatrix = Matrix4::Ortho(
            lightSpaceCenter.x - frustumRadius, lightSpaceCenter.x + frustumRadius, 
            lightSpaceCenter.y - frustumRadius, lightSpaceCenter.y + frustumRadius, 
            -maxZ - nearPullback, -minZ + farPullback);

        return lightProjectionMatrix;
    }

    void RenderSubsystem::ExecuteView(const View& view, const Vector<RenderCommand>& commands)
    {
        AX_ASSERT(view.renderTarget, "View has no render target!");
        AX_ASSERT(view.width != 0, "Invalid view render target dimension!");
        AX_ASSERT(view.height != 0, "Invalid view render target dimension!");

        RenderViewData renderViewData;
        renderViewData.cameraPosition = view.cameraPosition;
        renderViewData.viewProjectionMatrix = view.viewProjection;

        if(m_renderSceneData.hasDirectionalLight)
        {
            Matrix4 lightProj = ComputeShadowProjection(m_renderSceneData.lightViewMatrix, view.viewProjection);
            renderViewData.lightViewProjectionMatrix = lightProj * m_renderSceneData.lightViewMatrix;
            
            RenderShadowMap(renderViewData.lightViewProjectionMatrix, commands);
        }
        
        m_graphicsDevice->SetColorWriteEnabled(true);
        m_graphicsDevice->SetDepthWriteEnabled(true);
        m_graphicsDevice->SetDepthFunction(DepthFunction::Less);
        view.renderTarget->Bind();
        m_graphicsDevice->SetViewport(0, 0, view.width, view.height);
        m_graphicsDevice->SetClearColor(view.clearColor);
        m_graphicsDevice->Clear();

        RenderScenePass(m_renderSceneData, renderViewData, commands);
    }

    GraphicsDevice::API RenderSubsystem::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }
}
