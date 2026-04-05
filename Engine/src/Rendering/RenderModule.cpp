#include "Rendering/RenderModule.h"
#include "Core/Application.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

namespace axiom
{
    RenderModule::RenderModule(Application& application)
    : ApplicationModule(application)
    {
    }

    bool RenderModule::Initialize()
    {
        auto API = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();
        m_graphicsDevice = GraphicsDevice::Create(API, window);
        return true;
    }

    void RenderModule::Shutdown()
    {
        // m_renderer->Shutdown();
        // m_renderer.reset();
    }

    void RenderModule::Update()
    {
        // nothing for now
    }

    void RenderModule::Render()
    {
    }

    GraphicsDevice& RenderModule::GetGraphicsDevice() const
    {
        return *m_graphicsDevice;
    }

    void RenderModule::BeginScene(const OrtographicCamera& camera)
    {
        m_sceneData.viewProjectionMatrix = camera.GetViewProjectionMatrix();
        m_graphicsDevice->SetClearColor(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
        m_graphicsDevice->Clear();
    }

    void RenderModule::EndScene()
    {
        m_graphicsDevice->Present();
    }

    void RenderModule::Submit(const SharedPtr<VertexBuffer>& vb, const SharedPtr<IndexBuffer>& ib, const SharedPtr<Shader>& shader)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_sceneData.viewProjectionMatrix);
        m_graphicsDevice->DrawIndexed(vb, ib);
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