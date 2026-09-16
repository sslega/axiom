#include "InspectorPanel.h"
#include "UI/Selection.h"
#include "UI/Commands/UndoStack.h"
#include "UI/Commands/PropertyEditCommand.h"
#include "UI/Commands/AddComponentCommand.h"
#include "Scene/Entity.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    String ToTitleCase(const char* name)
    {
        String output = "";
        for (size_t i = 0; name[i] != '\0'; ++i)
        {
            char c = name[i];
            if(i == 0)
            {
                output.push_back(std::toupper(c));
                continue;
            }
            if(std::isupper(c))
            {
                output.push_back(' ');
                output.push_back(std::toupper(c));
                continue;
            }
            output.push_back(c);
        }
        return output;
    }

    void InspectorPanel::OnRender()
    {
        ImGui::Begin(GetName().c_str());

        Entity* entity = m_selection.Get();
        if(!entity)
        {
             ImGui::TextUnformatted("No entity selected");
             ImGui::End();
             return;
        }

        ImGui::TextUnformatted(entity->GetName().c_str());
        for (Component* component : entity->GetComponents())
        {
            const TypeDescriptor* typeDescriptor = Reflection::Get().Find(typeid(*component));
            if(!typeDescriptor)
            {
                ImGui::TextDisabled("<unreflected component>");
                continue;
            }

            ImGui::PushID(component);
            if (ImGui::CollapsingHeader(typeDescriptor->name, ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::BeginTable("Fields", 2);
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                for (const FieldDescriptor& field : typeDescriptor->fields)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(ToTitleCase(field.name).c_str());

                    ImGui::SetNextItemWidth(-FLT_MIN); 
                    ImGui::TableSetColumnIndex(1);

                    String label = "##" + String(field.name);
                    
                    if (field.type == FieldType::Resource)
                    {
                        ImGui::TextDisabled("<resource>");
                        continue;
                    }
                    
                    FieldValue value = field.get(component);
                    
                    switch (field.type)
                    {
                    case FieldType::Float:
                    {
                        float local = std::get<float>(value);
                        bool changed = ImGui::DragFloat(label.c_str(), &local, 0.1f);
                        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
                        break;
                    }
                    case FieldType::Int:
                    {
                        int local = std::get<int>(value);
                        bool changed = ImGui::DragInt(label.c_str(), &local, 0.1f);
                        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
                        break;
                    }
                    case FieldType::Bool:
                    {
                        bool local = std::get<bool>(value);
                        bool changed = ImGui::Checkbox(label.c_str(), &local);
                        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
                        break;
                    }
                    case FieldType::Vec3:
                    {
                        Vec3 local = std::get<Vec3>(value);
                        
                        bool activated = false;
                        bool changed = false;
                        bool deactivatedAfterEdit = false;
                        
                        auto drawAxis = [&](const char* axisName, float* axisValue, Vec3 axisColor) -> void
                        {
                            String axisLabel = axisName + label;
                            String axisId = label + "_" + axisName;
                            ImGui::AlignTextToFramePadding();

                            ImVec2 p = ImGui::GetCursorScreenPos();
                            ImGui::SetNextItemWidth(-FLT_MIN);
                            changed |= ImGui::DragFloat(axisId.c_str(), axisValue, 0.1f);

                            float stripWidth = 3.0f;
                            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + stripWidth, p.y + ImGui::GetFrameHeight()), IM_COL32(255 * axisColor.x, 255 * axisColor.y, 255 * axisColor.z, 255));
                            
                            if(ImGui::IsItemActivated())
                            {
                                activated |= true;
                            }
                            if(ImGui::IsItemDeactivatedAfterEdit())
                            {
                                deactivatedAfterEdit |= true;
                            }
                        };

                        ImGui::BeginTable("VecFields", 3);
                        ImGui::TableNextColumn();
                        drawAxis("X", &local.x, Vec3(1,0,0));
                        ImGui::TableNextColumn();
                        drawAxis("Y", &local.y, Vec3(0,1,0));
                        ImGui::TableNextColumn();
                        drawAxis("Z", &local.z, Vec3(0,0,1));
                        ImGui::EndTable();

                        CommitField(component, field, value, activated, changed, deactivatedAfterEdit, local);
                        break;
                    }
                    case FieldType::Color:
                    {
                        Color local = std::get<Color>(value);
                        bool changed = ImGui::ColorEdit3(label.c_str(), reinterpret_cast<float*>(&local));
                        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
                        break;
                    }
                    case FieldType::String:
                    {
                        char buffer[256];
                        String local = std::get<String>(value);
                        strncpy_s(buffer, sizeof(buffer), local.c_str(), _TRUNCATE);
                        bool changed = ImGui::InputText(label.c_str(), buffer, sizeof(buffer));
                        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), String(buffer));
                        break;
                    }
                    default: break;
                    }
                }
                ImGui::EndTable();
            }
            ImGui::PopID();
            ImGui::Spacing();
        }

        if (ImGui::Button("Add Component", ImVec2(-FLT_MIN, 0.0f)))

        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            for (const TypeDescriptor* componentTypeDescriptor : Reflection::Get().GetAllTypes<Component>())
            {
                if(entity->HasComponent(componentTypeDescriptor->type)) continue;

                if(ImGui::MenuItem(componentTypeDescriptor->name))
                {
                    m_undoStack.Execute(MakeUnique<AddComponentCommand>(*entity, *componentTypeDescriptor));
                }
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}