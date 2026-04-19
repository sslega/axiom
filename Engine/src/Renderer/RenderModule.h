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

        struct MeshBuffers { SharedPtr<VertexBuffer> vb; SharedPtr<IndexBuffer> ib; };
        UnorderedMap<MeshResource*, MeshBuffers> m_meshCache;

        MeshBuffers GetOrCreateBuffers(const SharedPtr<MeshResource>& mesh);
        SharedPtr<Shader> GetOrCreateInstancedShader(const SharedPtr<Shader>& shader);
        void SubmitInstanced(const MeshBuffers& buffers, const SharedPtr<Material>& material, const Vector<Matrix4>& transforms);
        void SubmitBatched(const SharedPtr<Material>& material, const Vector<RenderCommand>& commands);


        // SceneModule* m_sceneModule;

        // Vector<MeshComponent*> m_meshComponents;

        // RenderAPI m_renderApi;
        // UniquePtr<IRenderDevice> m_renderer;

        // RenderMesh* GetMesh(const SharedPtr<MeshResource> meshResource);
        // UniquePtr<RenderMesh> CreateMesh(const SharedPtr<MeshResource> meshResource);
        // void DestroyMesh(SharedPtr<MeshResource> meshResource);

        // RenderShader* GetShader(const SharedPtr<ShaderResource> shaderResource);
        // UniquePtr<RenderShader> CreateShader(const SharedPtr<ShaderResource> shaderResource);
        // void DestroyShader(SharedPtr<ShaderResource> shaderResource);

        // UnorderedMap<SharedPtr<MeshResource>, UniquePtr<RenderMesh>> m_meshes;
        // UnorderedMap<SharedPtr<ShaderResource>, UniquePtr<RenderShader>> m_shaders;
    };
}
