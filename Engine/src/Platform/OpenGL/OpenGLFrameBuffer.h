#pragma once

#include "Renderer/FrameBuffer.h"

namespace axiom
{
    class OpenGLFrameBuffer : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FramebufferSpec& spec);
        ~OpenGLFrameBuffer();

        virtual void Bind() override;
        virtual void Unbind() override;
        virtual void Resize(uint32 width, uint32 height) override;
        virtual uint32 GetColorAttachmentID() const override { return m_textureColorBuffer; }
        virtual uint32 GetNativeHandle() const override { return m_frameBuffer; }
    
    private:
        uint32 m_frameBuffer;
        uint32 m_textureColorBuffer;
        uint32 m_depthRenderBuffer;
    };
}