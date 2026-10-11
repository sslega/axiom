#include "Renderer/PostProcessTest.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderSubsystem.h"

namespace axiom
{
    void PostProcessTest::Execute(RenderSubsystem& renderer, FrameBuffer& source, FrameBuffer& target)
    {
        if(!m_shader)
        {
            m_shader = renderer.GetShader("engine://Shaders/UVDebug.glsl");
        }
        m_shader->Bind();
        m_shader->UploadUniform("u_ScreenTexture", 0);
        renderer.GetGraphicsDevice().BindFrameBufferTexture(source, 0);
        renderer.RenderFullScreenPass(target);
    }
}