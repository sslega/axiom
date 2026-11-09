#include "Renderer/OutlinePostProcess.h"
#include "Renderer/Shader.h"
#include "Renderer/RenderSubsystem.h"
#include <bit>

namespace axiom
{
    void OutlinePostProcess::Execute(const PostProcessContext& context)
    {
        if(!context.customMask) return;

        // Initialize JFA
        if(!m_JFAInitShader)
        {
            m_JFAInitShader = context.renderer.GetShader("engine://Shaders/JFAInit.glsl");
        }

        FramebufferSpec jFramebufferSpec;
        jFramebufferSpec.frameBufferFormat = FramebufferFormat::RG16;
        jFramebufferSpec.width = context.source.GetWidth();
        jFramebufferSpec.height = context.source.GetHeight();

        FrameBuffer& jfaFramebufferA = context.renderer.AcquireTemporaryFrameBuffer(jFramebufferSpec);
        FrameBuffer& jfaFramebufferB = context.renderer.AcquireTemporaryFrameBuffer(jFramebufferSpec);

        m_JFAInitShader->Bind();
        m_JFAInitShader->UploadUniform("u_CustomMaskTexture", 1);

        context.renderer.GetGraphicsDevice().BindFrameBufferTexture(*context.customMask, 1);
        context.renderer.RenderFullScreenPass(jfaFramebufferA);

        // JFA step
        if(!m_JFAStepShader)
        {
            m_JFAStepShader = context.renderer.GetShader("engine://Shaders/JFAStep.glsl");
        }

        uint32 thickness = 2;
        uint32 startStep = std::bit_ceil(static_cast<uint32>(thickness));
        m_JFAStepShader->Bind();
        m_JFAStepShader->UploadUniform("u_SeedTexture", 0);

        FrameBuffer* read  = &jfaFramebufferA;
        FrameBuffer* write = &jfaFramebufferB;
        FrameBuffer* jfaResult = nullptr;
        
        for(int k = startStep; k >= 1; k /= 2)
        {
            m_JFAStepShader->UploadUniform("u_StepSize", k);
            context.renderer.GetGraphicsDevice().BindFrameBufferTexture(*read, 0);
            context.renderer.RenderFullScreenPass(*write);
            jfaResult = write;
            std::swap(read, write);
        }

        // Output to temp render buffer
        if(!m_shader)
        {
            m_shader = context.renderer.GetShader("engine://Shaders/OutlinePostProcess.glsl");
        }

        FramebufferSpec tempFramebufferSpec;
        tempFramebufferSpec.frameBufferFormat = context.source.GetFramebufferFormat();
        tempFramebufferSpec.width = context.source.GetWidth();
        tempFramebufferSpec.height = context.source.GetHeight();

        FrameBuffer& tempFrameBuffer = context.renderer.AcquireTemporaryFrameBuffer(tempFramebufferSpec);

        m_shader->Bind();
        m_shader->UploadUniform("u_ScreenTexture", 0);
        m_shader->UploadUniform("u_SeedTexture", 1);
        m_shader->UploadUniform("u_SelectionColor",Vec4(0, 1, 0, 0));
        m_shader->UploadUniform("u_OutlineColor", Vec4(1, 1, 0, 1.0));
        m_shader->UploadUniform("u_OutlineThickness", int(thickness));

        context.renderer.GetGraphicsDevice().BindFrameBufferTexture(context.source, 0);
        context.renderer.GetGraphicsDevice().BindFrameBufferTexture(*jfaResult, 1);
        context.renderer.RenderFullScreenPass(tempFrameBuffer);

        // Output back to source
        context.renderer.GetGraphicsDevice().CopyFrameBuffer(tempFrameBuffer, context.source);

        // Release temp resources
        context.renderer.ReleaseTemporaryFrameBuffer(jfaFramebufferA);
        context.renderer.ReleaseTemporaryFrameBuffer(jfaFramebufferB);
        context.renderer.ReleaseTemporaryFrameBuffer(tempFrameBuffer);
    }
    
}