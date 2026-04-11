#include "Platform/GLFW/GLFWWindow.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Input/Keyboard.h"

namespace axiom
{
    GLFWWindow::GLFWWindow(const AppWindowConfig& config)
    : ApplicationWindow(config)
    {

        if (!glfwInit())
            assert("Failed to initialize GLFW");

        m_window = glfwCreateWindow(GetConfig().width, GetConfig().height, GetConfig().title.c_str(), NULL, NULL);
        if (!m_window)
        {
            assert("Failed to create GLFW window");
        }

        glfwSetWindowUserPointer(m_window, this);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);

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
            e.m_keyCode    = static_cast<KeyCode>(key);
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
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

    void GLFWWindow::OnPollEvents()
    {
        glfwPollEvents();
    }

    void* GLFWWindow::GetNativeWindow() const
    {
        return m_window;
    }

    void GLFWWindow::OnUpdate()
    {
    }

    void GLFWWindow::OnModulesInitialized()
    {
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void GLFWWindow::OnBeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GLFWWindow::OnEndFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(m_window);
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
        return GetConfig().width;
    }

    uint32 GLFWWindow::Height() const
    {
        return GetConfig().height;
    }

    float GLFWWindow::AspectRatio() const
    {
        return float(GetConfig().width) / float(GetConfig().height);
    }

    StringView GLFWWindow::Title() const
    {
        return GetConfig().title;
    }
}
