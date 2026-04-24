#pragma once

namespace axiom
{
    struct FramebufferSpec
    {
        uint32 width;
        uint32 height;
    };

    class FrameBuffer
    {
    public: 
        FrameBuffer(const FramebufferSpec& spec);
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;
        virtual uint32 GetColorAttachmentID() const = 0;
        virtual uint32 GetNativeHandle() const = 0;

        uint32 GetWidth() const { return m_width; }
        uint32 GetHeight() const { return m_height; }
    protected:
        uint32 m_width;
        uint32 m_height;
    };
}

