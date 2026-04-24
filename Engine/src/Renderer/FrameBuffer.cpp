#include "FrameBuffer.h"

namespace axiom
{
    FrameBuffer::FrameBuffer(const FramebufferSpec& spec)
    {
        m_width = spec.width;
        m_height = spec.height;
    }
}