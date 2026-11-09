#include "EditorUI.h"
#include "Panels/ViewportPanel.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/InspectorPanel.h"
#include "Viewport.h"
#include "Renderer/OutlinePostProcess.h"
#include "Platform/FileDialog.h"
#include <imgui.h>
#include <ImGuizmo.h>

namespace axiom
{
    EditorUI::EditorUI(RenderSubsystem& renderSubsystem, WorldSubsystem& worldSubsystem, FileSubsystem& fileSubsystem, ResourceSubsystem& resourceSubsystem)
    : m_renderSubsystem(renderSubsystem)
    , m_worldSubsystem(worldSubsystem)
    , m_fileSubsystem(fileSubsystem)
    , m_resourceSubsystem(resourceSubsystem)
    , m_selection(worldSubsystem.GetActiveScene())
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
        ImGuizmo::BeginFrame();
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

        const bool textInputActive = ImGui::GetIO().WantTextInput;
        // A gizmo drag holds pointers into the scene and an undo step in progress; these would pull both away.
        const bool sceneShortcutsEnabled = !textInputActive && !m_gizmoState.dragging;

        if (sceneShortcutsEnabled && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Z))
        {
            m_undoStack.Undo();
        }
        if (sceneShortcutsEnabled && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Y))
        {
            m_undoStack.Redo();
        }
        if (!textInputActive && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_R))
        {
            m_renderSubsystem.ReloadShaders();
        }
        // No key repeat: holding Ctrl+O must not reopen the dialog.
        if (sceneShortcutsEnabled && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_N, false))
        {
            NewScene();
        }
        if (sceneShortcutsEnabled && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_O, false))
        {
            OpenScene();
        }
        if (sceneShortcutsEnabled && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_S, false))
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

        LoadScene(*virtualPath);
    }

    bool EditorUI::LoadScene(const String& virtualPath)
    {
        if(!m_worldSubsystem.LoadScene(virtualPath))
        {
            return false;
        }
        OnSceneReplaced();
        return true;
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
        // Undo steps hold pointers into the old scene, and a reloaded file can bring back the selected ID.
        m_selection.Clear();
        m_undoStack.Clear();
    }

    ViewportPanel* EditorUI::AddViewportPanel(const String& name)
    {
        auto panel = MakeUnique<ViewportPanel>(name, m_renderSubsystem, m_selection, m_undoStack, m_gizmoState);
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
        m_panels.push_back(MakeUnique<InspectorPanel>(m_selection, m_undoStack, m_resourceSubsystem, m_fileSubsystem));
    }
}