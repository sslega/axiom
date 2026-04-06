#include "Platform/ApplicationWindow.h"
#include "Platform/Windows/Win32Window.h"
#include <GLFW/glfw3.h>
#include <assert.h>
#include "Event/WindowEvent.h"

namespace axiom
{
    UniquePtr<ApplicationWindow> ApplicationWindow::Create(const AppWindowConfig &config)
    {
        switch (config.backend)
        {
            case ApplicationWindowBackend::Win32:
                return MakeUnique<Win32Window>(config);
            case ApplicationWindowBackend::GLFW:
                return MakeUnique<GLFWWindow>(config);
            default:
                return nullptr;
        }
    }

    GLFWWindow::GLFWWindow(const AppWindowConfig& desc)
    {
        m_desc = desc;

        if (!glfwInit())
            assert("Failed to initialize GLFW");

        m_window = glfwCreateWindow(m_desc.width, m_desc.height, m_desc.title.c_str(), NULL, NULL);
        if (!m_window)
        {
            assert("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(m_window, this);

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            WindowEvent windowEvent(WindowEvent::RESIZE);
            windowEvent.m_newWidth = width;
            windowEvent.m_newHeight = height;
            self->DispatchEvent(windowEvent);
        });
    }

    GLFWWindow::~GLFWWindow()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

    void GLFWWindow::PoolEvents()
    {
        glfwPollEvents();
    }

    void* GLFWWindow::GetNativeWindow() const
    {
        return m_window;
    }

    void GLFWWindow::Update()
    {

    }

    void GLFWWindow::Render()
    {

    }

    bool GLFWWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void GLFWWindow::CloseWindow()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    uint32 GLFWWindow::Width() const
    {
        return m_desc.width;
    }

    uint32 GLFWWindow::Height() const
    {
        return m_desc.height;
    }

    float GLFWWindow::AspectRatio() const
    {
        return float(m_desc.width) / float(m_desc.height);
    }

    StringView GLFWWindow::Title() const
    {
        return m_desc.title;
    }
    
}
