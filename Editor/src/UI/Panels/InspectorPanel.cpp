#include "InspectorPanel.h"
#include "UI/Selection.h"
#include "Scene/Entity.h"
#include "Reflection/Reflection.h"

namespace axiom
{
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
            if (ImGui::CollapsingHeader(typeDescriptor->name))
            {
                for (const FieldDescriptor& field : typeDescriptor->fields)
                {
                    if (field.type == FieldType::Resource)
                    {
                        ImGui::TextDisabled("%s: <resource>", field.name);
                        continue;
                    }
                    
                    FieldValue value = field.get(component);
                    
                    switch (field.type)
                    {
                    case FieldType::Float:  ImGui::Text("%s: %.3f", field.name, std::get<float>(value)); break;
                    case FieldType::Int:    ImGui::Text("%s: %d",   field.name, std::get<int>(value));   break;
                    case FieldType::Bool:   ImGui::Text("%s: %s",   field.name, std::get<bool>(value) ? "true" : "false"); break;
                    case FieldType::Vec3:
                    {
                        const Vec3& v = std::get<Vec3>(value);
                        ImGui::Text("%s: %.3f, %.3f, %.3f", field.name, v.x, v.y, v.z);
                        break;
                    }
                    case FieldType::String: ImGui::Text("%s: %s", field.name, std::get<String>(value).c_str()); break;
                    default: break;
                    }
                }
            }
            ImGui::PopID();
            ImGui::Spacing();
        }

        ImGui::End();
    }
}