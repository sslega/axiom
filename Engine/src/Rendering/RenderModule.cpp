#include "Rendering/RenderModule.h"
// #include "Rendering/MeshComponent.h"
// #include "Rendering/RenderShader.h"
// #include "Rendering/Material.h"
// #include "Rendering/GL/GLRenderMesh.h"
// #include"Rendering/GL/GLRenderShader.h"
#include "Core/Assert.h"
// #include "Scene/SceneModule.h"
#include <iostream>
#include <cassert>

#include "Renderer/Buffer.h"
#include "Core/Application.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/OpenGL/OpenGLSwapChain.h"
#include "Platform/OpenGL/OpenGLRHI.h"
#include "Renderer/RenderResourceFactory.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace axiom
{
    RenderModule::RenderModule(Application& application)
    : ApplicationModule(application)
    {
    }

    bool RenderModule::Initialize()
    {    
        m_swapChain = CreateSwapChain();

        auto API = GetRenderAPI();
        m_graphicsDevice = GraphicsDevice::Create(API);
        m_renderResourceFactory = RenderResourceFactory::Create(API);


        uint32 triangleIndices[3] = {0, 1, 2};
        float triangleVertices [7 * 3] =
        {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
        };
        uint32 squareIndices[2 * 3] = {0, 1, 2, 2, 3, 0};
        float squareVertices [7 * 4] =
        {
            -0.75f, -0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
             0.75f, -0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
             0.75f,  0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
            -0.75f,  0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f
        };

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };

        auto& RRF = *m_renderResourceFactory;

        m_triangleVB = RRF.CreateVertexBuffer(triangleVertices, sizeof(triangleVertices));
        m_triangleVB->SetLayout(layout);
        m_triangleIB = RRF.CreateIndexBuffer(triangleIndices, 3);

        m_rectangleVB = RRF.CreateVertexBuffer(squareVertices, sizeof(squareVertices));
        m_rectangleVB->SetLayout(layout);
        m_rectangleIB = RRF.CreateIndexBuffer(squareIndices, 6);


        String vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        String fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                //color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_shader = RRF.CreateShader(vertexSrc, fragmentSrc);
        m_shader->Bind();

        // m_sceneModule = GetApp().GetModule<SceneModule>();
        // AX_ASSERT(m_sceneModule, "Scene Module not registered");
        
        // bool bInitialized = ApplicationModule::Initialize();
        // assert(bInitialized && "ApplicationModule initialization failed!");

        // m_renderApi = m_application.GetRenderAPI();            
        // m_renderer = CreateRenderDevice(m_renderApi);
        // assert(m_renderer && "Failed to create renderer API");
        
        // const IApplicationWindow& window = m_application.GetApplicationWindow();
        return true;
        // return m_renderer->Initialize(window.GetNativeWindow()) && bInitialized;
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
        auto& GFX = *m_graphicsDevice;

        GFX.SetClearColor(Vector4(0.5, 0.5, 0.5, 1.0));
        GFX.Clear();

        m_shader->Bind();

        GFX.DrawIndexed(m_rectangleVB, m_rectangleIB);
        GFX.DrawIndexed(m_triangleVB, m_triangleIB);

        m_swapChain->SwapBuffers();
    }

    GraphicsDevice::API RenderModule::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }

    UniquePtr<SwapChain> RenderModule::CreateSwapChain()
    {
        GraphicsDevice::API renderAPI = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();

        switch (renderAPI)
        {
            case GraphicsDevice::API::OpenGL: return MakeUnique<OpenGLSwapChain>(window);
            case GraphicsDevice::API::Vulkan: AX_ASSERT(false, "Unknown RenderAPI!");
        }
        
        return nullptr;        
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