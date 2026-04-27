#include "Material.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace axiom
{
    Material::Material(const SharedPtr<Shader> shader)
    : m_shader(shader)
    {
    }

    void Material::Bind(const Vector<String>& defines)
    {
        Shader* target = defines.empty()? m_shader.get(): m_shader->GetVariant(defines).get();

        target->Bind();
        for (auto& [name, value] : m_uniforms)
        {
            std::visit([&](auto& v){ target->UploadUniform(name, v); }, value);
        }
            
        for (auto& [name, pair] : m_textures)
        {
            auto& [texture, slot] = pair;
            texture->Bind();
            target->UploadUniform(name, (int)slot);
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