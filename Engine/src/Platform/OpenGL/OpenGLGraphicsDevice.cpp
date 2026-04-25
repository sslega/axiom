#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "Resources/Texture2DResource.h"
#include "Resources/MeshResource.h"
#include "Renderer/Vertex.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include "Core/Log.h"
#include "Resources/ShaderResource.h"
#include "OpenGLGraphicsDevice.h"



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

    void OpenGLGraphicsDevice::DrawIndexedInstanced(const SharedPtr<VertexBuffer> &vertexBuffer, const SharedPtr<IndexBuffer> &indexBuffer, const SharedPtr<VertexBuffer> &instanceBuffer, uint32 instanceCount)
    {
        uint32 vao = GetOrCreateVAO(vertexBuffer, instanceBuffer);
        glBindVertexArray(vao);
        indexBuffer->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
    }

    void OpenGLGraphicsDevice::SwapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

    SharedPtr<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(float* vertices, uint32 size) const
    {
        return MakeShared<OpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<VertexBuffer> OpenGLGraphicsDevice::CreateDynamicVertexBuffer(uint32 byteSize) const
    {
        return MakeShared<OpenGLVertexBuffer>(byteSize);
    }

    SharedPtr<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(const MeshResource& mesh) const
    {
        const auto& vertices = mesh.GetVertices();
        auto vb = CreateVertexBuffer(
            reinterpret_cast<float*>(const_cast<Vertex*>(vertices.data())),
            static_cast<uint32>(vertices.size() * sizeof(Vertex))
        );
        vb->SetLayout(Vertex::GetLayout());
        return vb;
    }

    SharedPtr<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(uint32* indices, uint32 count) const
    {
        return MakeShared<OpenGLIndexBuffer>(indices, count);
    }

    SharedPtr<IndexBuffer> OpenGLGraphicsDevice::CreateDynamicIndexBuffer(uint32 maxCount) const
    {
        return MakeShared<OpenGLIndexBuffer>(maxCount);
    }

    SharedPtr<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(const MeshResource& mesh) const
    {
        const auto& indices = mesh.GetIndices();
        return CreateIndexBuffer(
            const_cast<uint32*>(indices.data()),
            static_cast<uint32>(indices.size())
        );
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

    SharedPtr<FrameBuffer> OpenGLGraphicsDevice::CreateFrameBuffer(const FramebufferSpec &spec) const
    {
        return MakeShared<OpenGLFrameBuffer>(spec);
    }

    void OpenGLGraphicsDevice::SetDepthTestEnabled(bool enabled)
    {
        if(enabled)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        
    }

    void OpenGLGraphicsDevice::BindFrameBufferTexture(FrameBuffer& frameBuffer, uint32 slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, frameBuffer.GetColorAttachmentID());
    }

    uint32 OpenGLGraphicsDevice::GetOrCreateVAO(const SharedPtr<VertexBuffer> &vertexBuffer)
    {
        return GetOrCreateVAO(vertexBuffer, nullptr);
    }

    uint32 OpenGLGraphicsDevice::GetOrCreateVAO(const SharedPtr<VertexBuffer>& vertexBuffer, const SharedPtr<VertexBuffer>& instanceBuffer)
    {
        auto key = std::make_pair(vertexBuffer.get(), instanceBuffer ? instanceBuffer.get() : nullptr);
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

        if (instanceBuffer)
        {
            instanceBuffer->Bind();
            const BufferLayout& instanceLayout = instanceBuffer->GetLayout();
            uint8 instanceAttribBase = static_cast<uint8>(layout.GetSize());
            for (uint8 i = 0; i < instanceLayout.GetSize(); ++i)
            {
                const auto& element = instanceLayout.GetElement(i);
                uint8 index = instanceAttribBase + i;
                glEnableVertexAttribArray(index);
                glVertexAttribPointer(
                    index,
                    element.GetComponentCount(),
                    ShaderDataTypeToOpenGLBaseType(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    instanceLayout.GetStride(),
                    (const void*)element.offset
                );
                glVertexAttribDivisor(index, 1);
            }
        }

        m_vaoCache[key] = vao;
        return vao;
    }
}
