#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
#include "Core/Assert.h"

namespace axiom
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpec& spec) : FrameBuffer(spec)
    {
        int internalFormat;
        int format;
        int type;
        GetFrameBufferParameters(m_frameBufferFormat, internalFormat, format, type);

        // Create render buffer object
        glGenFramebuffers(1, &m_frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        
        if(IsDepthOnly())
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        else
        {
            // Color texture attachement
            glGenTextures(1, &m_textureColorBuffer);
            glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, type, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorBuffer, 0);
        }
        
        // Depth and stencil render buffer
        glGenTextures(1, &m_textureDepthBuffer);
        glBindTexture(GL_TEXTURE_2D, m_textureDepthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (IsDepthOnly())
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_textureDepthBuffer, 0);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            AX_ASSERT(false, "Framebuffer is not complete!");
        }
	    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);    
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_frameBuffer);
        glDeleteTextures(1, &m_textureColorBuffer);
        glDeleteTextures(1, &m_textureDepthBuffer);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    }

    void OpenGLFrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void OpenGLFrameBuffer::Resize(uint32 width, uint32 height)
    {
        m_width = width;
        m_height = height;

        int internalFormat;
        int format;
        int type;
        GetFrameBufferParameters(m_frameBufferFormat, internalFormat, format, type);

        if(!IsDepthOnly())
        {
            glBindTexture(GL_TEXTURE_2D, m_textureColorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, type, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glBindTexture(GL_TEXTURE_2D, m_textureDepthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

    }
    void OpenGLFrameBuffer::GetFrameBufferParameters(const FramebufferFormat& frameBufferFormat, int& internalFormat, int& format, int& type)
    {
            switch (frameBufferFormat)
            {
            case FramebufferFormat::RGB8:
                internalFormat = GL_RGB8;
                format = GL_RGB;
                type = GL_UNSIGNED_BYTE;
                break;
            case FramebufferFormat::RGBA8:
                internalFormat = GL_RGBA8;
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
                break;
            case FramebufferFormat::RG16:
                internalFormat = GL_RG16;
                format = GL_RG;
                type = GL_FLOAT;
                break;
            case FramebufferFormat::RG16F:
                internalFormat = GL_RG16F;
                format = GL_RG;
                type = GL_FLOAT;
                break;
            case FramebufferFormat::Depth:
                internalFormat = 0;
                format = 0;
                type = 0;
                break;
            case  FramebufferFormat::Undefined:
                AX_ASSERT(false, "Undefined FramebufferFormat");
                break;
            default:
                AX_ASSERT(false, "No valid FramebufferFormat");
                break;
            }
    }
}