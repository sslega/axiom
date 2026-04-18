#include "Material.h"
#include "Renderer/Shader.h"

namespace axiom
{
    void Material::Bind()
    {
        m_shader->Bind();
        for (auto& [name, value] : m_uniforms)
        {
            std::visit([&](auto& v){ m_shader->UploadUniform(name, v); }, value);
        }
    }
    
    void Material::Unbind()
    {
        m_shader->Unbind();
    }
}