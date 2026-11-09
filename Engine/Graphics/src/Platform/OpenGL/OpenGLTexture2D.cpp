#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "Resources/Texture2DResource.h"
#include "Core/Assert.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace axiom
{
    OpenGLTexture2D::OpenGLTexture2D(const String& path)
    : m_path(path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        AX_ASSERT(data, "Failed to load image!");

        m_width = width;
        m_height = height;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        glTextureStorage2D(m_rendererID, 1, GL_RGBA8, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const Texture2DResource& resource)
    {
        m_width  = resource.GetWidth();
        m_height = resource.GetHeight();

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        glTextureStorage2D(m_rendererID, 1, GL_RGBA8, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, resource.GetData());
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_rendererID);
    }

    void OpenGLTexture2D::Bind(uint32 slot) const
    {
        glBindTextureUnit(slot, m_rendererID);
    }
}
