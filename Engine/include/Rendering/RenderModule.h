#pragma once

#include "Core/ApplicationModule.h"
#include "Rendering/RenderTypes.h"
#include "Rendering/RenderMesh.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Renderer/RHI.h"
#include "Renderer/SwapChain.h"
#include "Renderer/GraphicsDevice.h"

namespace axiom
{

    class IndexBuffer;
    class VertexBuffer;
    class Shader;

    class RenderModule : public ApplicationModule
    {
    public:
        RenderModule(Application& engine);

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

        GraphicsDevice::API GetRenderAPI() const;

    private:
        // UniquePtr<RenderContext> m_renderContext;
        // UniquePtr<RendererAPI> m_renderAPI;
        UniquePtr<GraphicsDevice> m_graphicsDevice;
        UniquePtr<SwapChain> m_swapChain;

        SharedPtr<VertexBuffer> m_triangleVB;
        SharedPtr<IndexBuffer>  m_triangleIB;
        SharedPtr<VertexBuffer> m_rectangleVB;
        SharedPtr<IndexBuffer>  m_rectangleIB;
        SharedPtr<Shader> m_shader;

        SharedPtr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32 size);
        SharedPtr<IndexBuffer> CreateIndexBuffer(uint32* indices, uint32 count);
        SharedPtr<Shader> CreateShader(const String& vertexSource, const String& fragmentSource);

        // UniquePtr<RHI> CreateRHI();
        UniquePtr<SwapChain> CreateSwapChain();
        UniquePtr<GraphicsDevice> CreateGraphicsDevice();

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