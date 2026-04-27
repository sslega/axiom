#include "Renderer/Shader.h"
#include "Math/Matrix4.h"

namespace axiom
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& vertexSource, const String& fragmentSource);
        ~OpenGLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void UploadUniform(const String& name, const int& value) override;
        virtual void UploadUniform(const String& name, const float& value) override;
        virtual void UploadUniform(const String& name, const Vec2& value) override;
        virtual void UploadUniform(const String& name, const Vec3& value) override;
        virtual void UploadUniform(const String& name, const Vec4& value) override;
        virtual void UploadUniform(const String& name, const Matrix4& value) override;
        virtual SharedPtr<Shader> GetVariant(const Vector<String>& defines) override;

    private:
        uint32 m_rendererID;

        String m_vertexSource;
        String m_fragmentSource;
        StringMap<SharedPtr<Shader>> m_variantCache;
    };
}