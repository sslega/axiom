#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include <imgui.h>

namespace axiom
{
    EditorUI::EditorUI(RenderSubsystem &renderSubsystem)
    :m_renderSubsystem (renderSubsystem)
    {
    }

    void EditorUI::OnInitialize()
    {
        AddViewportPanel("ViewportA");
        AddViewportPanel("ViewportB");
    }

    void EditorUI::OnUpdate(float dt)
    {
        for(auto& panel : m_panels)
        {
            panel->OnUpdate(dt);
        }
    }

    void EditorUI::OnRender()
    {
        ImGui::DockSpaceOverViewport();

        if(ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::EndMenu();
            } 
            ImGui::EndMainMenuBar();
        }
        

        for(auto& panel : m_panels)
        {
            panel->OnRender();
        }
    }
    
    void EditorUI::AddViewportPanel(const String& name)
    {
        m_panels.push_back(MakeUnique<ViewportPanel>(name, m_renderSubsystem));
    }
}