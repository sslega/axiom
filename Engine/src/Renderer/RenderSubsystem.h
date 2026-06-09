#pragma once

#include "Core/ApplicationSubsystem.h"
#include "Renderer/RenderTypes.h"
#include "Renderer/RenderMesh.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Camera.h"
#include "Renderer/View.h"
#include "Core/Timestep.h"

namespace axiom
{
    class IndexBuffer;
    class VertexBuffer;
    class Shader;
    class MaterialResource;
    class RenderResourceFactory;
    class FrameBuffer;

    class RenderSubsystem : public ApplicationSubsystem
    {
    public:
        RenderSubsystem(Application& engine);

        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<MaterialResource>& material, const Matrix4& transform);
        void Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader, const Matrix4& transform);

        SharedPtr<MaterialResource> GetMaterial(const String path);
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

        void ReloadShaders();

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
        struct RenderSceneData
        {
            Matrix4 viewProjectionMatrix;
            Matrix4 lightViewProjectionMatrix;
            
            bool hasDirectionalLight = false;
            Vec3 lightDirection;
            Vec3 lightColor;

            Vec3 cameraPosition;
            float time;
        };

        struct RenderCommand
        {
            SharedPtr<MeshResource> mesh;
            SharedPtr<MaterialResource> material;
            Matrix4 transform;
        };

        UniquePtr<GraphicsDevice> m_graphicsDevice;
        
        RenderSceneData m_renderSceneData;

        StringMap<SharedPtr<Shader>> m_shaderCache;

        using InstanceGroupKey  = std::pair<MeshResource*, MaterialResource*>;   // for OnRender grouping
        using InstanceMaterialBufferKey = std::pair<VertexBuffer*, MaterialResource*>;   // for SubmitInstanced cache
        using InstanceShaderBufferKey = std::pair<VertexBuffer*, Shader*>;

        PairMap<InstanceMaterialBufferKey, SharedPtr<VertexBuffer>> m_instanceMaterialBufferCache;
        PairMap<InstanceShaderBufferKey, SharedPtr<VertexBuffer>> m_instanceShaderBufferCache;
        UnorderedMap<MaterialResource*, SharedPtr<VertexBuffer>> m_batchVBCache;
        UnorderedMap<MaterialResource*, SharedPtr<IndexBuffer>> m_batchIBCache;

        struct MeshBuffers { SharedPtr<VertexBuffer> vb; SharedPtr<IndexBuffer> ib; };
        UnorderedMap<MeshResource*, MeshBuffers> m_meshCache;

        MeshBuffers GetOrCreateBuffers(const SharedPtr<MeshResource>& mesh);

        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<MaterialResource>& material, const Vector<Matrix4>& transforms);
        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Shader>& instancedShader, const Vector<Matrix4>& transforms);
        void SubmitBatched(const SharedPtr<MaterialResource>& material, const Vector<RenderCommand>& commands);
        void ResetDebugDrawCounters();

        SharedPtr<Shader> m_screenQuadShader;        
        
        SharedPtr<MaterialResource> m_debugDrawMaterial;
        SharedPtr<Shader> m_errorShader;

        SharedPtr<VertexBuffer> m_screenQuadVB;
        SharedPtr<IndexBuffer> m_screenQuadIB;
        void RenderToScreen();

        void RenderShadowPass(const Matrix4& lightProjectionMatrix, const Vector<RenderCommand>& commands);
        void RenderScenePass(const Matrix4& viewProjectionMatrix, const Vector<RenderCommand>& commands);

        Vector<Vec3> GetFrustumCornersWorldSpace(const Matrix4& invViewProj);
        Matrix4 ComputeShadowProjection(const Matrix4& lightViewMatrix, const Matrix4& cameraViewProjection);

        Vector<View> BuildViews(Scene& scene);
        void ExecuteView(const View& view, const Vector<RenderCommand>& commands);

        SharedPtr<FrameBuffer> m_frameBuffer;
        SharedPtr<FrameBuffer> m_shadowMapFrameBuffer;

        uint32 m_callCount = 0;
        uint32 m_instanceCallCount = 0;
        uint32 m_instanceObjectCount = 0;
        uint32 m_batchCallCount = 0;
        uint32 m_batchObjectCount = 0;

        bool m_batchingEnabled = true;
        bool m_instancingEnabled = true;
        int m_debugDrawMode = 0;
        
        TimePoint m_lastRenderTime;
        float m_elapsedTime = 0;
    };
}
