#include "InspectorPanel.h"
#include "UI/Selection.h"
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
                        if(ImGui::DragFloat(label.c_str(), &local, 0.1f))
                        {
                            field.set(component, FieldValue(local));
                        }
                        break;
                    }  
                    case FieldType::Int:
                    {
                        int local = std::get<int>(value);
                        if(ImGui::DragInt(label.c_str(), &local, 0.1f))
                        {
                            field.set(component, FieldValue(local));
                        }
                        break;
                    }
                    case FieldType::Bool:
                    {
                        bool local = std::get<bool>(value);
                        if(ImGui::Checkbox(label.c_str(), &local))
                        {
                            field.set(component, FieldValue(local));
                        }
                        break;
                    }   
                    case FieldType::Vec3:
                    {
                        Vec3 local = std::get<Vec3>(value);

                        auto drawAxis = [&](const char* axisName, float* axisValue, Vec3 axisColor) -> bool
                        {
                            String axisLabel = axisName + label;
                            String axisId = label + "_" + axisName;
                            ImGui::AlignTextToFramePadding();

                            ImVec2 p = ImGui::GetCursorScreenPos();
                            ImGui::SetNextItemWidth(-FLT_MIN);
                            bool modified = ImGui::DragFloat(axisId.c_str(), axisValue, 0.1f);

                            float stripWidth = 3.0f;
                            ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + stripWidth, p.y + ImGui::GetFrameHeight()), IM_COL32(255 * axisColor.x, 255 * axisColor.y, 255 * axisColor.z, 255));
                            return modified;
                        };

                        bool changed = false;
                        ImGui::BeginTable("VecFields", 3);
                        ImGui::TableNextColumn();
                        changed |= drawAxis("X", &local.x, Vec3(1,0,0));
                        ImGui::TableNextColumn();
                        changed |= drawAxis("Y", &local.y, Vec3(0,1,0));
                        ImGui::TableNextColumn();
                        changed |= drawAxis("Z", &local.z, Vec3(0,0,1));
                        ImGui::EndTable();

                        if (changed)
                        field.set(component, FieldValue(local));
                        break;
                    }
                    case FieldType::Color:
                    {
                        Color local = std::get<Color>(value);
                        if(ImGui::ColorEdit3(label.c_str(), reinterpret_cast<float*>(&local)))
                        {
                            field.set(component, FieldValue(local));
                        }
                        break;
                    }
                    case FieldType::String:
                    {
                        char buffer[256];
                        String local = std::get<String>(value);
                        strncpy_s(buffer, sizeof(buffer), local.c_str(), _TRUNCATE);
                        if(ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
                        {
                            field.set(component, FieldValue(String(buffer)));
                        }
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

        ImGui::End();
    }
}