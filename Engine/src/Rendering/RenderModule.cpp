#include "Rendering/RenderModule.h"
#include "Core/Assert.h"
#include <iostream>
#include <cassert>

#include "Renderer/Buffer.h"
#include "Core/Application.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Renderer/Camera.h"
#include "Math/Math.h"
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
        auto API = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();
        m_graphicsDevice = GraphicsDevice::Create(API, window);

        float ratio = GetApp().GetApplicationWindow().AspectRatio();

        m_camera = OrtographicCamera(-2.0f, 2.0f, -2.0f / ratio, 2.0f / ratio);
        m_camera.SetRotation(ToRadians(45.0f));


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

        auto& GFX = *m_graphicsDevice;

        m_triangleVB = GFX.CreateVertexBuffer(triangleVertices, sizeof(triangleVertices));
        m_triangleVB->SetLayout(layout);
        m_triangleIB = GFX.CreateIndexBuffer(triangleIndices, 3);

        m_rectangleVB = GFX.CreateVertexBuffer(squareVertices, sizeof(squareVertices));
        m_rectangleVB->SetLayout(layout);
        m_rectangleIB = GFX.CreateIndexBuffer(squareIndices, 6);


        String vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

        m_shader = GFX.CreateShader(vertexSrc, fragmentSrc);
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

    //TODO: Temporary untill proper scene is created
    void RenderModule::Render()
    {
        BeginScene(m_camera);
        Submit(m_rectangleVB, m_rectangleIB, m_shader);
        Submit(m_triangleVB,  m_triangleIB,  m_shader);
        EndScene();
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