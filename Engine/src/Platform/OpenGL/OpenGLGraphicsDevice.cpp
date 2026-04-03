#include "Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/ApplicationWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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

        printf("OpenGLSwapChain initialized\n");
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

    SharedPtr<VertexBuffer> OpenGLGraphicsDevice::CreateVertexBuffer(float* vertices, uint32 size)
    {
        return MakeShared<OpenGLVertexBuffer>(vertices, size);
    }

    SharedPtr<IndexBuffer> OpenGLGraphicsDevice::CreateIndexBuffer(uint32* indices, uint32 count)
    {
        return MakeShared<OpenGLIndexBuffer>(indices, count);
    }

    SharedPtr<Shader> OpenGLGraphicsDevice::CreateShader(const String& vertexSource, const String& fragmentSource)
    {
        return MakeShared<OpenGLShader>(vertexSource, fragmentSource);
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