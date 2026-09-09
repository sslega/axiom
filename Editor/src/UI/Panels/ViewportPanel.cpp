#include "ViewportPanel.h"
#include "Renderer/RenderSubsystem.h"
#include "Viewport.h"

namespace axiom
{
    ViewportPanel::ViewportPanel(const String& name, RenderSubsystem& renderSubsystem)
    : ViewportPanel(name, 320, 240, renderSubsystem)
    {
    }

    ViewportPanel::ViewportPanel(const String &name, uint32 width, uint32 height, RenderSubsystem &renderSubsystem)
    : EditorPanel(name), m_renderSubsystem(renderSubsystem)
    {
        axiom::FramebufferSpec frameBufferSpec;
        frameBufferSpec.width = width;
        frameBufferSpec.height = height;
        frameBufferSpec.depthOnly = false;

        SharedPtr<FrameBuffer> frameBuffer = renderSubsystem.GetGraphicsDevice().CreateFrameBuffer(frameBufferSpec);
        
        m_viewport = MakeUnique<Viewport>(frameBuffer, m_name);
    }

    void ViewportPanel::OnRender()
    {
        m_viewport->OnRender();
        m_renderSubsystem.SubmitView(m_viewport->GetView());

        ImGui::Begin(m_viewport->GetName().c_str());
        m_viewport->SetActive(ImGui::IsWindowFocused());
        ImVec2 available = ImGui::GetContentRegionAvail();

        uint32 desiredW = (uint32)available.x;
        uint32 desiredH = (uint32)available.y;

        Vec2 windowSize = Vec2(available.x, available.y);
        Vec2 viewportSize = Vec2(m_viewport->GetWidth(), m_viewport->GetHeight());
        if (desiredW > 0 && 
            desiredH > 0 &&
            (desiredW != m_viewport->GetWidth() || desiredH != m_viewport->GetHeight()))
        {
            m_viewport->Resize(desiredW, desiredH);
        }

        uint32 colorId = m_viewport->GetFrameBuffer()->GetColorAttachmentID();
        ImGui::Image((ImTextureID)(uintptr_t)colorId,
            ImVec2(desiredW, desiredH),
            ImVec2(0,1), ImVec2(1,0));
        ImGui::End();
    }
    
    void ViewportPanel::OnUpdate(float deltaTime)
    {
        m_viewport->OnUpdate(deltaTime);
    }
}