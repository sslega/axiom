#include "OutlinerPanel.h"
#include "Scene/Entity.h"
#include "Scene/WorldSubsystem.h"
#include "UI/Selection.h"

namespace axiom
{
    void OutlinerPanel::OnRender()
    {
        ImGui::Begin("Outliner");
        
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

        ImGui::End();
    }
}