#include "ImGui/ImGuiSubsystem.h"
#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderSubsystem.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace axiom
{
    ImGuiSubsystem::ImGuiSubsystem(Application& application)
    : ApplicationSubsystem(application)
    {
    }

    void ImGuiSubsystem::OnInitialize()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();

        GLFWwindow* window = static_cast<GLFWwindow*>(
            GetApp().GetApplicationWindow().GetNativeWindow()
        );
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void ImGuiSubsystem::OnShutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiSubsystem::OnBeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiSubsystem::OnRender()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiSubsystem::OnEndFrame()
    {
    
    }
}
