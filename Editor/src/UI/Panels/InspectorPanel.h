#pragma once
#include "UI/EditorPanel.h"

namespace axiom
{
    class Selection;

    class InspectorPanel : public EditorPanel
    {
    public:
        InspectorPanel(Selection& selection): 
        EditorPanel("Inspector"), m_selection(selection) {}

        virtual void OnRender() override;
    private:
        Selection& m_selection;
    };
}