#include "OutlinerPanel.h"
#include "Scene/Entity.h"
#include "Scene/WorldSubsystem.h"
#include "UI/Selection.h"
#include "UI/Commands/UndoStack.h"
#include "UI/Commands/CreateEntityCommand.h"

namespace axiom
{
    OutlinerPanel::OutlinerPanel(WorldSubsystem &world, Selection &selection, UndoStack &undoStack)
    : EditorPanel("Outliner"), m_world(world), m_selection(selection), m_undoStack(undoStack)
    {
    }

    void OutlinerPanel::OnRender()
    {
        ImGui::Begin(GetName().c_str());
        
        for(Entity* e : m_world.GetActiveScene().GetEntities())
        {
            bool isSelected = m_selection.IsSelected(e);
            String label = e->GetName();
            if(label.length() == 0)
            {
                label = "<unnamed>";
            }
            ImGui::PushID(e->GetID().c_str()); 
            if(ImGui::Selectable(label.c_str(), isSelected))
            {
                m_selection.Set(e);
            }
            ImGui::PopID();
        }

        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("OutlinerContextMenu");
        }

        if (ImGui::BeginPopup("OutlinerContextMenu"))
        {
            if (ImGui::MenuItem("Create empty Entity"))
            {
                Scene& scene = m_world.GetActiveScene();
                UniquePtr<CreateEntityCommand> command = MakeUnique<CreateEntityCommand>(scene);
                Entity* entity = command->GetEntity();
                m_undoStack.Execute(std::move(command));
                m_selection.Set(entity);
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}