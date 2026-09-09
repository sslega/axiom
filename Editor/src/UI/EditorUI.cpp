#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include "Panels/OutlinerPanel.h"
#include <imgui.h>

namespace axiom
{
    EditorUI::EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem)
    : m_renderSubsystem(renderSubsystem)
    , m_worldSubsystem(worldSubsystem)
    {
    }

    void EditorUI::OnInitialize()
    {
        AddViewportPanel("ViewportA");
        AddViewportPanel("ViewportB");
        AddOutlinerPanel();
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
    
    void EditorUI::AddOutlinerPanel()
    {
        m_panels.push_back(MakeUnique<OutlinerPanel>(m_worldSubsystem, m_selection));
    }
}