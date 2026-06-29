#include "Renderer/Shader.h"
#include "Math/Matrix4.h"

namespace axiom
{
    class OpenGLShader : public Shader
    {
    public:
        ~OpenGLShader();

        static SharedPtr<OpenGLShader> Create(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap);

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual bool Reload(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap) override;


        virtual void UploadUniform(const String& name, const int& value) override;
        virtual void UploadUniform(const String& name, const float& value) override;
        virtual void UploadUniform(const String& name, const Vec2& value) override;
        virtual void UploadUniform(const String& name, const Vec3& value) override;
        virtual void UploadUniform(const String& name, const Vec4& value) override;
        virtual void UploadUniform(const String& name, const Matrix4& value) override;
        virtual SharedPtr<Shader> GetVariant(const Vector<String>& defines) override;
        virtual bool IsValid() const override;


    private:
        OpenGLShader(const String& vertexSource, const String& fragmentSource,  const Vector<String>& sourceMap, const uint32 shaderID);

        String m_vertexSource;
        String m_fragmentSource;
        Vector<String> m_sourceMap;
        uint32 m_shaderID;
        StringMap<SharedPtr<Shader>> m_variantCache;
        bool m_isValid;

        static bool Compile(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap, uint32& shaderID);
        static String ResolveSourceIndices(const String& errorLog, const Vector<String>& sourceMap);
    };
}