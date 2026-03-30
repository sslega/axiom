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
#include "Platform/OpenGL/OpenGLSwapChain.h"
#include "Platform/OpenGL/OpenGLRHI.h"
#include "Core/Application.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
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
        m_RHI = CreateRHI();
        m_swapChain = CreateSwapChain();



        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        float vertices [3 * 3] =
        {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };

        m_vertexbuffer = CreateVertexBuffer(vertices, sizeof(vertices));
        m_vertexbuffer->Bind();

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"}
        };

        for(uint8 index = 0; index < layout.GetSize(); ++index)
        {
            auto& element = layout.GetElement(index);
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index, 
                element.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(element.type), 
                element.normalized ? GL_TRUE : GL_FALSE, 
                layout.GetStride(), 
                (const void*)element.offset
            );
        }

        uint32 indices[3] = {0, 1, 2};
        m_indexBuffer = CreateIndexBuffer(indices, 3);
        m_indexBuffer->Bind();

        String vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        String fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
            }
        )";

        m_shader = CreateShader(vertexSrc, fragmentSrc);
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
        
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindVertexArray(m_vertexArray);
        // m_shader->Bind();
        // m_vertexbuffer->Bind();
        // m_indexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(static_cast<GLFWwindow*>(GetApp().GetApplicationWindow().GetNativeWindow()));



        // m_shader->Bind();
        
        // m_indexBuffer->Bind();
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        // m_swapChain->SwapBuffers();
        // m_renderContext->SwapBuffers();
        // Scene* activeScene = m_sceneModule->GetActiveScene();
        // if (!activeScene) return;

        // m_meshComponents.clear();
        // activeScene->GetComponents<MeshComponent>(m_meshComponents);
        
        // m_renderer->BeginFrame();

        // for (MeshComponent* meshComponent : m_meshComponents)
        // {
        //     if(!meshComponent->IsVisible()) continue;

        //     SharedPtr<MeshResource> meshResource = meshComponent->GetMesh();
        //     if(!meshResource) continue;

        //     SharedPtr<Material> material = meshComponent->GetMaterial();
        //     if(!material) continue;

        //     RenderMesh* mesh = GetMesh(meshResource);
        //     RenderShader* shader = GetShader(material->shader);
            
        //     shader->Bind();
        //     mesh->Bind();
        //     // Draw
        //     mesh->Unbind();
        //     shader->Unbind();

        // }

        // m_renderer->EndFrame();
    }

    RenderAPI RenderModule::GetRenderAPI() const
    {
        return GetApp().GetRenderAPI();
    }

    SharedPtr<IndexBuffer> RenderModule::CreateIndexBuffer(uint32 *indices, uint32 count)
    {
        RenderAPI renderAPI = GetRenderAPI();
        switch (renderAPI)
        {
            case RenderAPI::OpenGL: return MakeShared<OpenGLIndexBuffer>(indices, count);
            default: AX_ASSERT(false, "Unknown RenderAPI!");
        }
        return nullptr;
    }

    SharedPtr<VertexBuffer> RenderModule::CreateVertexBuffer(float* vertices, uint32 size)
    {
        RenderAPI renderAPI = GetRenderAPI();
        switch (renderAPI)
        {
            case RenderAPI::OpenGL: return MakeShared<OpenGLVertexBuffer>(vertices, size);
            default: AX_ASSERT(false, "Unknown RenderAPI!");
        }
        return nullptr;
    }

    SharedPtr<Shader> RenderModule::CreateShader(const String &vertexSource, const String &fragmentSource)
    {
        RenderAPI renderAPI = GetRenderAPI();
        switch (renderAPI)
        {
            case RenderAPI::OpenGL: return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
            default: AX_ASSERT(false, "Unknown RenderAPI!");
        }
        return nullptr;
    }

    UniquePtr<RHI> RenderModule::CreateRHI()
    {
        RenderAPI renderAPI = GetRenderAPI();

        switch (renderAPI)
        {
            case RenderAPI::OpenGL: return MakeUnique<OpenGLRHI>();
            default: AX_ASSERT(false, "Unknown RenderAPI!");
        }
        return nullptr;
    }

    UniquePtr<SwapChain> RenderModule::CreateSwapChain()
    {
        RenderAPI renderAPI = GetRenderAPI();
        auto& window = GetApp().GetApplicationWindow();

        switch (renderAPI)
        {
            case RenderAPI::OpenGL: return MakeUnique<OpenGLSwapChain>(window);
            case RenderAPI::Vulkan: AX_ASSERT(false, "Unknown RenderAPI!");
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