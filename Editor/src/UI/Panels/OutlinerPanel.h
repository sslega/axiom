#pragma once
#include "UI/EditorPanel.h"

namespace axiom
{
    class WorldSubsystem;
    class Selection;

    class OutlinerPanel : public EditorPanel
    {
    public:
        OutlinerPanel(WorldSubsystem& world, Selection& selection): 
        EditorPanel("Outliner"), m_world(world), m_selection(selection) {}

        virtual void OnRender() override;
    
    private:
        WorldSubsystem& m_world;
        Selection& m_selection;
    };
}