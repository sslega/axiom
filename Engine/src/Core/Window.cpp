#include "Axiom/Core/Window.h"
#include "Axiom/Core/Assert.h"
#include "GLFW/glfw3.h"

using namespace ax;

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

void ax::Window::Update()
{
    glfwPollEvents();
}

void ax::Window::Render()
{
    ensure(m_window, "GLFW window not initialized");

    glfwSwapBuffers(m_window);
}

bool ax::Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void ax::Window::CloseWindow()
{
    ensure(m_window, "GLFW window not initialized");
    
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    glfwTerminate();
    
}
