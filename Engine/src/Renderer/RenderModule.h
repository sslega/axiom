#pragma once

#include "Core/ApplicationModule.h"
#include "Renderer/RenderTypes.h"
#include "Renderer/RenderMesh.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Camera.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class Material;
    class RenderResourceFactory;

    class RenderModule : public ApplicationModule
    {
    public:
        RenderModule(Application& engine);

        void BeginScene();
        void EndScene();
        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Material>& material, const Matrix4& transform);
        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform);

        SharedPtr<Shader> GetShader(const String path);

        GraphicsDevice& GetGraphicsDevice() const;
        GraphicsDevice::API GetRenderAPI() const;

        inline uint32 GetDrawCallCount() const { return m_batchCallCount + m_instanceCallCount; }
        inline uint32 GetBatchCallCount() const { return m_batchCallCount; }
        inline uint32 GetBatchObjectCount() const { return m_batchObjectCount; }
        inline uint32 GetInstanceCallCount() const { return m_instanceCallCount; }
        inline uint32 GetInstanceObjectCount() const { return m_instanceObjectCount; }

    protected:
        void OnInitialize() override;
        void OnShutdown() override;
        void OnUpdate() override;
        void OnRender() override;

        SharedPtr<Shader> CreateShader(const String path);
        SharedPtr<Shader> CreateInstancedShader(const String path);

    private:
        struct SceneData
        {
            Matrix4 viewProjectionMatrix;
        };

        struct RenderCommand
        {
            SharedPtr<MeshResource> mesh;
            SharedPtr<Material> material;
            Matrix4 transform;
        };

        UniquePtr<GraphicsDevice> m_graphicsDevice;
        SceneData m_sceneData;

        StringMap<SharedPtr<Shader>> m_shaderCache;
        UnorderedMap<Shader*, SharedPtr<Shader>> m_instancedShaderCache;

        using InstanceGroupKey  = std::pair<MeshResource*, Material*>;   // for OnRender grouping
        using InstanceBufferKey = std::pair<VertexBuffer*, Material*>;   // for SubmitInstanced cache

        PairMap<InstanceBufferKey, SharedPtr<VertexBuffer>> m_instanceBufferCache;
        UnorderedMap<Material*, SharedPtr<VertexBuffer>> m_batchVBCache;
        UnorderedMap<Material*, SharedPtr<IndexBuffer>> m_batchIBCache;

        struct MeshBuffers { SharedPtr<VertexBuffer> vb; SharedPtr<IndexBuffer> ib; };
        UnorderedMap<MeshResource*, MeshBuffers> m_meshCache;

        MeshBuffers GetOrCreateBuffers(const SharedPtr<MeshResource>& mesh);
        SharedPtr<Shader> GetOrCreateInstancedShader(const SharedPtr<Shader>& shader);
        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Material>& material, const Vector<Matrix4>& transforms);
        void SubmitBatched(const SharedPtr<Material>& material, const Vector<RenderCommand>& commands);
        void ResetDebugDrawCounters();

        uint32 m_instanceCallCount = 0;
        uint32 m_instanceObjectCount = 0;
        uint32 m_batchCallCount = 0;
        uint32 m_batchObjectCount = 0;
    };
}
