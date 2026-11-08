#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/InspectorPanel.h"
#include "Viewport.h"
#include "Renderer/OutlinePostProcess.h"
#include "Platform/FileDialog.h"
#include <imgui.h>

namespace axiom
{
    EditorUI::EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem, FileSubsystem& fileSubsystem)
    : m_renderSubsystem(renderSubsystem)
    , m_worldSubsystem(worldSubsystem)
    , m_fileSubsystem(fileSubsystem)
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
                if(ImGui::MenuItem("New Scene", "Ctrl+N", false))
                {
                    NewScene();
                }
                if(ImGui::MenuItem("Open Scene", "Ctrl+O", false))
                {
                    OpenScene();
                }
                if(ImGui::MenuItem("Save Scene", "Ctrl+S", false))
                {
                    SaveScene();
                }
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
        // No key repeat: holding Ctrl+O must not reopen the dialog.
        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_N, false))
        {
            NewScene();
        }
        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_O, false))
        {
            OpenScene();
        }
        if (!ImGui::GetIO().WantTextInput && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false))
        {
            SaveScene();
        }

        // must run after RenderSubsystem::BeginScene, before RenderSubsystem::OnRender
        if(Entity* selectedEntity = m_selection.Get())
        {
            m_renderSubsystem.AddTransientRenderFlags(*selectedEntity, RenderFlags::CustomMask);
        }
    }
    
    void EditorUI::NewScene()
    {
        m_worldSubsystem.NewScene();
        OnSceneReplaced();
    }

    void EditorUI::OpenScene()
    {
        std::optional<Path> physicalPath = FileDialog::OpenFile(m_fileSubsystem.Resolve("project://Scenes"));
        if(!physicalPath)
        {
            return;
        }

        std::optional<String> virtualPath = m_fileSubsystem.ToVirtual(*physicalPath);
        if(!virtualPath)
        {
            Log::Error("Scene is outside every mounted folder: {}", physicalPath->string());
            return;
        }

        if(m_worldSubsystem.LoadScene(*virtualPath))
        {
            OnSceneReplaced();
        }
    }

    void EditorUI::SaveScene()
    {
        const String& currentPath = m_worldSubsystem.GetActiveScenePath();
        if(!currentPath.empty())
        {
            m_worldSubsystem.SaveScene(currentPath);
            return;
        }

        std::optional<Path> physicalPath = FileDialog::SaveFile(m_fileSubsystem.Resolve("project://Scenes"));
        if(!physicalPath)
        {
            return;
        }

        std::optional<String> virtualPath = m_fileSubsystem.ToVirtual(*physicalPath);
        if(!virtualPath)
        {
            Log::Error("Scene is outside every mounted folder: {}", physicalPath->string());
            return;
        }

        m_worldSubsystem.SaveScene(*virtualPath);
    }

    void EditorUI::OnSceneReplaced()
    {
        // Both hold pointers into the old scene.
        m_selection.Clear();
        m_undoStack.Clear();
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