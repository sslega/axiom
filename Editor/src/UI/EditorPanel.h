#pragma once
#include "Core/Types.h"
#include <imgui.h>

namespace axiom
{
    class EditorPanel
    {
    public:
        EditorPanel(const String& name) : m_name(name){};
        virtual ~EditorPanel() = default;

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() = 0;
        virtual const String& GetName() { return m_name; };
    protected:
        String m_name = "EditorPanel";
    };
}