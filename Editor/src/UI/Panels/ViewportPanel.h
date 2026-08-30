#pragma once

#include "UI/EditorPanel.h"

namespace axiom
{
    class RenderSubsystem;
    class Viewport;
    class FrameBuffer;

    class ViewportPanel : public EditorPanel
    {
    public:
        ViewportPanel(const String& name, RenderSubsystem& renderSubsystem);
        ViewportPanel(const String& name, uint32 width, uint32 height, RenderSubsystem& renderSubsystem);

        virtual void OnRender() override;
        virtual void OnUpdate(float deltaTime) override;

        virtual const String& GetName() const override { return m_name; };
    
    private:
        RenderSubsystem& m_renderSubsystem;
        UniquePtr<Viewport> m_viewport;
        String m_name;
    };
}