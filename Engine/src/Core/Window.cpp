#include "axiom/core/Window.h"
#include "GLFW/glfw3.h"
#include <assert.h>

namespace axiom
{
    Window::Window(WindowDesc desc)
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

    void Window::Update()
    {
        glfwPollEvents();
    }

    void Window::Render()
    {
        assert(m_window);

        glfwSwapBuffers(m_window);
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::CloseWindow()
    {
        assert(m_window);
        
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
        
    }
}