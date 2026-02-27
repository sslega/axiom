#include "axiom/Platform/ApplicationWindow.h"
#include "axiom/Platform/Windows/Win32Window.h"
#include <GLFW/glfw3.h>
#include <assert.h>

namespace axiom
{
    UniquePtr<IApplicationWindow> IApplicationWindow::Create(const ApplicationWindowConfig &config)
    {
        switch (config.backend)
        {
            case ApplicationWindowBackend::Win32:
                return std::make_unique<Win32Window>(config);
            case ApplicationWindowBackend::GLFW:
                return std::make_unique<ApplicationWindow>(config);
            default:
                return nullptr;
        }
    }

    ApplicationWindow::ApplicationWindow(const ApplicationWindowConfig& desc)
    {
        m_desc = desc;

        if (!glfwInit())
            assert("Failed to initialize GLFW");

        m_window = glfwCreateWindow(m_desc.width, m_desc.height, m_desc.title.c_str(), NULL, NULL);
        if (!m_window)
        {
            assert("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
    }

    ApplicationWindow::~ApplicationWindow()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

    void ApplicationWindow::PoolEvents()
    {
        glfwPollEvents();
    }

    void* ApplicationWindow::GetNativeWindow() const
    {
        return m_window;
    }

    void ApplicationWindow::Update()
    {

    }

    void ApplicationWindow::Render()
    {

    }

    bool ApplicationWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void ApplicationWindow::CloseWindow()
    {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    uint32 ApplicationWindow::Width() const
    {
        return m_desc.width;
    }

    uint32 ApplicationWindow::Height() const
    {
        return m_desc.height;
    }

    StringView ApplicationWindow::Title() const
    {
        return m_desc.title;
    }
}
