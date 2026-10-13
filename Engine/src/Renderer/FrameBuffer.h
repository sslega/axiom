#pragma once

namespace axiom
{
    enum class FramebufferFormat { Undefined, R8, RGB8, RGBA8, RG16, RG16F, Depth };


    struct FramebufferSpec
    {
        uint32 width {0};
        uint32 height {0};
        FramebufferFormat frameBufferFormat {FramebufferFormat::Undefined};
    };

    class FrameBuffer
    {
    public: 
        FrameBuffer(const FramebufferSpec& spec);
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;
        virtual uint32 GetColorAttachmentID() const = 0;
        virtual uint32 GetDepthAttachmentID() const = 0;
        virtual uint32 GetNativeHandle() const = 0;

        uint32 GetWidth() const { return m_width; }
        uint32 GetHeight() const { return m_height; }
        FramebufferFormat GetFramebufferFormat() const { return m_frameBufferFormat; }
        bool IsDepthOnly() const {return m_frameBufferFormat == FramebufferFormat::Depth; }
        bool Matches(const FramebufferSpec& spec) const
        {
            return m_width == spec.width
                && m_height == spec.height
                && m_frameBufferFormat == spec.frameBufferFormat;
        }

        bool operator==(const FrameBuffer&) const = default;

    protected:
        uint32 m_width;
        uint32 m_height;
        FramebufferFormat m_frameBufferFormat;
    };
}

