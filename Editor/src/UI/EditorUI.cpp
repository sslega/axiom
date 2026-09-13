#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/InspectorPanel.h"
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
        AddInspectorPanel();
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
            if (ImGui::BeginMenu("Edit"))
            {
                if(ImGui::MenuItem("Undo", "Ctrl+Z", false, m_undoStack.CanUndo()))
                {
                    m_undoStack.Undo();
                }
                if(ImGui::MenuItem("Redo", "Ctrl+Y", false, m_undoStack.CanRedo()))
                {
                    m_undoStack.Redo();
                }
                ImGui::EndMenu();
            }  
            ImGui::EndMainMenuBar();
        }
        
        for(auto& panel : m_panels)
        {
            panel->OnRender();
        }

        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Z))
        {
            m_undoStack.Undo();
        }
        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Y))
        {
            m_undoStack.Redo();
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
    
    void EditorUI::AddInspectorPanel()
    {
        m_panels.push_back(MakeUnique<InspectorPanel>(m_selection, m_undoStack));
    }
}