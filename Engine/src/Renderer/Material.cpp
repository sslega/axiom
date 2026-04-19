#include "Material.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace axiom
{
    Material::Material(const SharedPtr<Shader> shader)
    : m_shader(shader)
    {
    }

    void Material::Bind()
    {
        m_shader->Bind();
        for (auto& [name, value] : m_uniforms)
        {
            std::visit([&](auto& v){ m_shader->UploadUniform(name, v); }, value);
        }
        for (auto& [name, pair] : m_textures)
        {
            auto& [texture, slot] = pair;
            texture->Bind();
            m_shader->UploadUniform(name, slot);
        }
    }
    
    void Material::Unbind()
    {
        m_shader->Unbind();
    }
    
    void Material::SetTexture(const String &name, const SharedPtr<Texture2D> texture, uint32 slot)
    {
        m_textures[name] = {texture, slot};
    }
}