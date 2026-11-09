#pragma once
#include "Core/Types.h"
#include "Math/Vector.h"
#include "Math/Matrix4.h"
#include "Resources/Resource.h"
#include <variant>

namespace axiom
{
    class Shader;
    class Texture2D;

    class MaterialResource : public Resource
    {
    public:
        MaterialResource(const SharedPtr<Shader> shader);

        void Bind(const Vector<String>& defines = {});
        void Unbind();
        void SetTexture(const String& name, const SharedPtr<Texture2D> texture, uint32 slot);
        bool IsValid();
        inline const SharedPtr<Shader> GetShader() const { return m_shader; }

        template<typename T>
        void SetUniform(const String& name, const T& value)
        {
            m_uniforms[name] = value;
        }

    private:
        SharedPtr<Shader> m_shader;
        using UniformValue = std::variant<int, float, Vec2, Vec3, Vec4, Matrix4>;
        StringMap<UniformValue> m_uniforms;
        StringMap<std::pair<SharedPtr<Texture2D>, uint32>> m_textures;
    };
}
