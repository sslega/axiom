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

    InspectorPanel::InspectorPanel(Selection &selection, UndoStack &undoStack)
    : EditorPanel("Inspector"), m_selection(selection), m_undoStack(undoStack)
    {
        m_fieldDrawFunction[FieldType::Bool] = &InspectorPanel::DrawBool;
        m_fieldDrawFunction[FieldType::Int] = &InspectorPanel::DrawInt;
        m_fieldDrawFunction[FieldType::Float] = &InspectorPanel::DrawFloat;
        m_fieldDrawFunction[FieldType::String] = &InspectorPanel::DrawString;
        m_fieldDrawFunction[FieldType::Vec3] = &InspectorPanel::DrawVec3;
        m_fieldDrawFunction[FieldType::Color] = &InspectorPanel::DrawColor;
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
                    auto it = m_fieldDrawFunction.find(field.type);
                    AX_ASSERT(it != m_fieldDrawFunction.end(), "No valid fieldDrawFunction registered");
                    (this->*it->second)(component, field, label);
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

    void InspectorPanel::DrawFloat(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
        float local = std::get<float>(value);
        bool changed = ImGui::DragFloat(label.c_str(), &local, 0.1f);
        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
    }

    void InspectorPanel::DrawBool(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
        bool local = std::get<bool>(value);
        bool changed = ImGui::Checkbox(label.c_str(), &local);
        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
    }

    void InspectorPanel::DrawInt(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
        int local = std::get<int>(value);
        bool changed = ImGui::DragInt(label.c_str(), &local, 0.1f);
        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
    }
    
    void InspectorPanel::DrawString(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
        char buffer[256];
        String local = std::get<String>(value);
        strncpy_s(buffer, sizeof(buffer), local.c_str(), _TRUNCATE);
        bool changed = ImGui::InputText(label.c_str(), buffer, sizeof(buffer));
        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), String(buffer));
    }

    void InspectorPanel::DrawVec3(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
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
    }

    void InspectorPanel::DrawColor(Component *component, const FieldDescriptor &field, const String &label)
    {
        FieldValue value = field.get(component);
        Color local = std::get<Color>(value);
        bool changed = ImGui::ColorEdit3(label.c_str(), reinterpret_cast<float*>(&local));
        CommitField(component, field, value, ImGui::IsItemActivated(), changed, ImGui::IsItemDeactivatedAfterEdit(), local);
    }
}