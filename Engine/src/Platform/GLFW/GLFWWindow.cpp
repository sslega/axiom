#include "Platform/GLFW/GLFWWindow.h"
#include <GLFW/glfw3.h>
#include <assert.h>
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"

namespace axiom
{
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
            WindowResizeEvent e;
            e.m_newWidth = width;
            e.m_newHeight = height;
            self->DispatchEvent(e);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            // if (action != (dGLFW_PRESS | GLFW_RELEASE | GLFW_REPEAT))
            //     return;

            auto* self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            KeyboardEvent e;
            e.m_keyCode    = static_cast<uint16>(key);
            e.m_charCode   = static_cast<uint16>(scancode);
            e.m_altKey     = mods & GLFW_MOD_ALT;
            e.m_controlKey = mods & GLFW_MOD_CONTROL;
            e.m_shiftKey   = mods & GLFW_MOD_SHIFT;
            e.m_action     = static_cast<KeyboardEvent::Action>(action);
            self->DispatchEvent(e);
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
