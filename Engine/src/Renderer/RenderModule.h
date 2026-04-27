#pragma once

#include "Core/ApplicationModule.h"
#include "Renderer/RenderTypes.h"
#include "Renderer/RenderMesh.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Camera.h"
#include "Core/Timestep.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class Material;
    class RenderResourceFactory;
    class FrameBuffer;

    class RenderModule : public ApplicationModule
    {
    public:
        RenderModule(Application& engine);

        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Material>& material, const Matrix4& transform);
        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform);
        
        SharedPtr<Material> GetMaterial(const String path);
        SharedPtr<Shader> GetShader(const String path);

        GraphicsDevice& GetGraphicsDevice() const;
        GraphicsDevice::API GetRenderAPI() const;

        inline void SetBatchingEnabled(bool value) { m_batchingEnabled = value; };
        inline void SetInstancingEnabled(bool value) { m_instancingEnabled = value; };
        inline void SetDebugDrawMode(int value) { m_debugDrawMode = value; }

        inline uint32 GetDrawCallCount() const { return m_callCount; }
        inline uint32 GetBatchCallCount() const { return m_batchCallCount; }
        inline uint32 GetBatchObjectCount() const { return m_batchObjectCount; }
        inline uint32 GetInstanceCallCount() const { return m_instanceCallCount; }
        inline uint32 GetInstanceObjectCount() const { return m_instanceObjectCount; }

    protected:
        void OnInitialize() override;
        void OnShutdown() override;
        void OnUpdate(float deltaTime) override;
        void OnBeginFrame() override;
        void OnRender() override;
        void OnEndFrame() override;

        void BeginScene();
        void EndScene();

        virtual void OnGUI();

        SharedPtr<Shader> CreateShader(const String path);

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
        UnorderedMap<Shader*, SharedPtr<Shader>> m_depthPassShaderCache;
        UnorderedMap<Shader*, SharedPtr<Shader>> m_depthPassInstancedShaderCache;

        using InstanceGroupKey  = std::pair<MeshResource*, Material*>;   // for OnRender grouping
        using InstanceMaterialBufferKey = std::pair<VertexBuffer*, Material*>;   // for SubmitInstanced cache
        using InstanceShaderBufferKey = std::pair<VertexBuffer*, Shader*>;

        PairMap<InstanceMaterialBufferKey, SharedPtr<VertexBuffer>> m_instanceMaterialBufferCache;
        PairMap<InstanceShaderBufferKey, SharedPtr<VertexBuffer>> m_instanceShaderBufferCache;
        UnorderedMap<Material*, SharedPtr<VertexBuffer>> m_batchVBCache;
        UnorderedMap<Material*, SharedPtr<IndexBuffer>> m_batchIBCache;

        struct MeshBuffers { SharedPtr<VertexBuffer> vb; SharedPtr<IndexBuffer> ib; };
        UnorderedMap<MeshResource*, MeshBuffers> m_meshCache;

        MeshBuffers GetOrCreateBuffers(const SharedPtr<MeshResource>& mesh);
        SharedPtr<Shader> CreateDepthPassShader(const String path);
        SharedPtr<Shader> CreateDepthPassInstancedShader(const String path);
        SharedPtr<Shader> GetOrCreateDepthPassShader(const SharedPtr<Shader>& shader);
        SharedPtr<Shader> GetOrCreateDepthPassInstancedShader(const SharedPtr<Shader>& shader);

        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Material>& material, const Vector<Matrix4>& transforms);
        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Shader>& instancedShader, const Vector<Matrix4>& transforms);
        void SubmitBatched(const SharedPtr<Material>& material, const Vector<RenderCommand>& commands);
        void ResetDebugDrawCounters();

        SharedPtr<Shader> m_screenQuadShader;        
        
        SharedPtr<Material> m_debugDrawMaterial;

        SharedPtr<VertexBuffer> m_screenQuadVB;
        SharedPtr<IndexBuffer> m_screenQuadIB;
        void RenderToScreen();

        SharedPtr<FrameBuffer> m_frameBuffer;

        uint32 m_callCount = 0;
        uint32 m_instanceCallCount = 0;
        uint32 m_instanceObjectCount = 0;
        uint32 m_batchCallCount = 0;
        uint32 m_batchObjectCount = 0;

        bool m_batchingEnabled = true;
        bool m_instancingEnabled = true;
        int m_debugDrawMode = 0;
        

        TimePoint m_lastRenderTime;
        float m_dt;
    };
}
