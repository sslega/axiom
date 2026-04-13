#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Resources/Texture2DResource.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include "Core/Log.h"
#include "Resources/ShaderResource.h"


namespace axiom
{
    OpenGLGraphicsDevice::OpenGLGraphicsDevice(const ApplicationWindow& window)
    {
        m_windowHandle = static_cast<GLFWwindow*>(window.GetNativeWindow());
        AX_ASSERT(m_windowHandle, "WindowHandle is null!");

        glfwMakeContextCurrent(m_windowHandle);

        int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        AX_ASSERT(gladStatus, "Failed to initialize Glad!");

        int width, height;
        glfwGetFramebufferSize(m_windowHandle, &width, &height);
        glViewport(0, 0, width, height);

        Log::Info("OpenGLGraphicsDevice initialized");
    }

    OpenGLGraphicsDevice::~OpenGLGraphicsDevice()
    {
        for (auto& [vb, vao] : m_vaoCache)
            glDeleteVertexArrays(1, &vao);
    }

    void OpenGLGraphicsDevice::SetClearColor(const Vec4 &color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void OpenGLGraphicsDevice::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLGraphicsDevice::DrawIndexed(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<IndexBuffer>& indexBuffer)
    {
        uint32 vao = GetOrCreateVAO(vertexBuffer);
        glBindVertexArray(vao);
        indexBuffer->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLGraphicsDevice::Present()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    // void OpenGLGraphicsDevice::InitImGuiRenderer()
    // {
    //     ImGui_ImplOpenGL3_Init("#version 330");
    // }

    // void OpenGLGraphicsDevice::ShutdownImGuiRenderer()
    // {
    //     ImGui_ImplOpenGL3_Shutdown();
    // }

    // void OpenGLGraphicsDevice::BeginImGuiRendererFrame()
    // {
    //     ImGui_ImplOpenGL3_NewFrame();
    // }

    // void OpenGLGraphicsDevice::EndImGuiFrame()
    // {
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // }

    SharedPtr<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(float* vertices, uint32 size) const
    {
        return MakeShared<OpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(uint32* indices, uint32 count) const
    {
        return MakeShared<OpenGLIndexBuffer>(indices, count);
    }

    SharedPtr<Shader> OpenGLGraphicsDevice::CreateShader(const String& vertexSource, const String& fragmentSource) const
    {
        return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
    }

    SharedPtr<Shader> OpenGLGraphicsDevice::CreateShader(const ShaderResource &shaderResource) const
    {
        return CreateShader(shaderResource.GetVertexSource(), shaderResource.GetFragmentSource());
    }

    SharedPtr<Texture2D> OpenGLGraphicsDevice::CreateTexture2D(const String& path) const
    {
        return MakeShared<OpenGLTexture2D>(path);
    }

    SharedPtr<Texture2D> OpenGLGraphicsDevice::CreateTexture2D(const Texture2DResource& resource) const
    {
        return MakeShared<OpenGLTexture2D>(resource);
    }

    uint32 OpenGLGraphicsDevice::GetOrCreateVAO(const SharedPtr<VertexBuffer> &vertexBuffer)
    {
        VertexBuffer* key = vertexBuffer.get();
        auto it = m_vaoCache.find(key);
        if (it != m_vaoCache.end())
            return it->second;

        uint32 vao;
        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);
        vertexBuffer->Bind();

        const BufferLayout& layout = vertexBuffer->GetLayout();
        for (uint8 index = 0; index < layout.GetSize(); ++index)
        {
            const auto& element = layout.GetElement(index);
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

        m_vaoCache[key] = vao;
        return vao;
    }
}