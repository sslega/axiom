#pragma once

#include "AxiomEngine.h"
#include "UI/EditorPanel.h"

using namespace axiom;

namespace axiom
{
    
    class EditorUI
    {
    public:
        EditorUI(RenderSubsystem& renderSubsystem);
        
        void OnInitialize();
        void OnUpdate(float dt);
        void OnRender();
    
    private:
        RenderSubsystem& m_renderSubsystem;
        Vector<UniquePtr<EditorPanel>> m_panels;
        
        void AddViewportPanel(const String& name);
    };
}