#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/InspectorPanel.h"
#include "Viewport.h"
#include "Renderer/OutlinePostProcess.h"
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
        ViewportPanel* ViewportA = AddViewportPanel("ViewportA");
        ViewportPanel* ViewportB = AddViewportPanel("ViewportB");
        ViewportB->GetViewport()->AddPostProcess<OutlinePostProcess>();
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
                if(ImGui::MenuItem("Reload Shaders", "Ctrl+R", false))
                {
                    m_renderSubsystem.ReloadShaders();
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
        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_R))
        {
            m_renderSubsystem.ReloadShaders();
        }

        // must run after RenderSubsystem::BeginScene, before RenderSubsystem::OnRender
        if(Entity* selectedEntity = m_selection.Get())
        {
            m_renderSubsystem.AddTransientRenderFlags(*selectedEntity, RenderFlags::CustomMask);
        }
    }
    
    ViewportPanel* EditorUI::AddViewportPanel(const String& name)
    {
        auto panel = MakeUnique<ViewportPanel>(name, m_renderSubsystem);
        ViewportPanel* raw = panel.get();
        m_panels.push_back(std::move(panel));
        return raw;
    }
    
    void EditorUI::AddOutlinerPanel()
    {
        m_panels.push_back(MakeUnique<OutlinerPanel>(m_worldSubsystem, m_selection, m_undoStack));
    }
    
    void EditorUI::AddInspectorPanel()
    {
        m_panels.push_back(MakeUnique<InspectorPanel>(m_selection, m_undoStack));
    }
}