#include "OutlinerPanel.h"
#include "Scene/Entity.h"
#include "Scene/WorldSubsystem.h"
#include "Selection.h"
#include "Commands/UndoStack.h"
#include "Commands/CreateEntityCommand.h"
#include "Commands/PropertyEditCommand.h"
#include "Commands/DeleteEntityCommand.h"
#include "UI/EditorIcons.h"
#include "UI/EditorTheme.h"

#include <IconsFontAwesome6.h>

namespace axiom
{
    OutlinerPanel::OutlinerPanel(WorldSubsystem &world, Selection &selection, UndoStack &undoStack)
    : EditorPanel("Outliner"), m_world(world), m_selection(selection), m_undoStack(undoStack)
    {
    }

    void OutlinerPanel::OnRender()
    {
        ImGui::Begin(EditorTheme::PanelTitle(ICON_FA_LIST, GetName()).c_str());
        
        for(Entity* e : m_world.GetActiveScene().GetEntities())
        {
            bool isSelected = m_selection.IsSelected(e);
            String label = e->GetName();
            if(label.length() == 0)
            {
                label = "<unnamed>";
            }
            ImGui::PushID(e->GetID().c_str());
            if(m_renamingEntity == e)
            {
                ImGui::SetNextItemWidth(-FLT_MIN);
                if(m_setRenamingFocus)
                {
                    ImGui::SetKeyboardFocusHere();
                    m_setRenamingFocus = false;
                }
                ImGui::InputText("", m_renameBuffer, sizeof(m_renameBuffer));
                if (ImGui::IsItemDeactivatedAfterEdit())
                {
                    if(const TypeDescriptor* typeDescriptor = Reflection::Get().Find(typeid(*e)))
                    {
                        if(const FieldDescriptor* fieldDescriptor = typeDescriptor->FindField("name"))
                        {
                            FieldValue oldValue = fieldDescriptor->get(e);
                            FieldValue newValue = FieldValue(String(m_renameBuffer));
                            m_undoStack.Execute(MakeUnique<PropertyEditCommand>(e, fieldDescriptor, oldValue, newValue));
                            m_renamingEntity = nullptr;
                        }
                    }
                }
                else if (ImGui::IsItemDeactivated())
                {
                    m_renamingEntity = nullptr;
                }
            }
            else
            {
                const String row = String(EditorIcons::ForEntity(*e)) + "  " + label;
                if(ImGui::Selectable(row.c_str(), isSelected, ImGuiSelectableFlags_None, ImVec2(0.0f, EditorTheme::kListRowHeight)))
                {
                    m_selection.Set(e);
                }
            }
            if(!m_renamingEntity && ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_renamingEntity = e;
                m_setRenamingFocus = true;
                strncpy_s(m_renameBuffer, sizeof(m_renameBuffer), e->GetName().c_str(), _TRUNCATE);
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
        if(ImGui::IsWindowFocused() && m_renamingEntity == nullptr)
        {
            if(ImGui::IsKeyPressed(ImGuiKey_Delete))
            {
                Scene& scene = m_world.GetActiveScene();
                Entity* entity = m_selection.Get();
                if(entity)
                {
                    m_undoStack.Execute(MakeUnique<DeleteEntityCommand>(scene, *entity));
                }
            }
        }
        EditorTheme::EndPanel();
    }
}