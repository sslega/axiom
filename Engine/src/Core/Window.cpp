#include "axiom/core/Window.h"
#include "GLFW/glfw3.h"
#include <assert.h>

namespace axiom
{
    GlfwWindow::GlfwWindow(WindowDesc desc)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("GLFW init failed");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
        glfwSwapInterval(desc.vsync);
        
        m_window = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
        
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
    }

    void GlfwWindow::Update()
    {
        glfwPollEvents();
    }

    void GlfwWindow::Render()
    {
        assert(m_window);

        glfwSwapBuffers(m_window);
    }

    bool GlfwWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void GlfwWindow::CloseWindow()
    {
        assert(m_window);
        
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        
    }

    std::uint32_t GlfwWindow::Width() const
    {
        int width;
        glfwGetWindowSize(m_window, &width, nullptr);
        return static_cast<std::uint32_t>(width);
    }

    std::uint32_t GlfwWindow::Height() const
    {
        int height;
        glfwGetWindowSize(m_window, nullptr, &height);
        return static_cast<std::uint32_t>(height);
    }

    std::string_view GlfwWindow::Title() const
    {
        return std::string_view(glfwGetWindowTitle(m_window));
    }
}