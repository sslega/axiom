#include "Viewport.h"

namespace axiom
{
    Viewport::Viewport(SharedPtr<FrameBuffer> frameBuffer, String viewportName)
    : m_name(viewportName)
    , m_frameBuffer(frameBuffer)
    {
    }
    
    void Viewport::OnUpdate(float deltaTime)
    {
        if(m_isActive)
        {
            m_camera.OnUpdate(deltaTime);
        }
    }
    
    void Viewport::OnRender()
    {
        m_view.cameraPosition = m_camera.GetCamera().GetPosition();
        m_view.clearColor = Vec4(0.1f, 0.1f, 0.1f, 1.0f);
        m_view.name = m_name;
        m_view.width = m_frameBuffer->GetWidth();
        m_view.height = m_frameBuffer->GetHeight();
        m_view.renderTarget = m_frameBuffer;
        m_view.viewProjection = m_camera.GetCamera().GetViewProjectionMatrix();
    }
    
    void Viewport::Resize(uint32 width, uint32 height)
    {
        AX_ASSERT(width > 0 ,"Viewport width cannot be 0.");
        AX_ASSERT(height > 0 ,"Viewport height cannot be 0.");

        m_frameBuffer->Resize(width, height);

        float aspectRatio = float(width) / float(height);
        m_camera.SetAspectRatio(aspectRatio);
        m_width = width;
        m_height = height;
    }
}