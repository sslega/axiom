#include "MaterialResource.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace axiom
{
    MaterialResource::MaterialResource(const SharedPtr<Shader> shader)
    : m_shader(shader)
    {
    }

    void MaterialResource::Bind(const Vector<String>& defines)
    {
        Shader* variant = defines.empty() ? nullptr : m_shader->GetVariant(defines).get();
        Shader* target = variant ? variant : m_shader.get();

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

    void MaterialResource::Unbind()
    {
        m_shader->Unbind();
    }

    void MaterialResource::SetTexture(const String& name, const SharedPtr<Texture2D> texture, uint32 slot)
    {
        m_textures[name] = {texture, slot};
    }
    
    bool MaterialResource::IsValid()
    {
        return m_shader->IsValid();
    }
}
