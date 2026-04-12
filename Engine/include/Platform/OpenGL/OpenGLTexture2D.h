#pragma once

#include "Renderer/Texture.h"

namespace axiom
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(const String& path);
        virtual ~OpenGLTexture2D();
        
        virtual uint32 GetWidth() const override { return m_width; }
        virtual uint32 GetHeight() const override { return m_height; }

        virtual void Bind(uint32 slot = 0) const override;
    private:
        uint32 m_width;
        uint32 m_height;
        String m_path;
        uint32 m_rendererID;
    };
}