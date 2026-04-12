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

        virtual void UploadUniformInt(const String& name,const int& value) override;
        virtual void UploadUniformMat4(const String& name,const Matrix4& value) override;
        virtual void UploadUniformVec4(const String& name, const Vec4& value) override;
    
    private:
        uint32 m_rendererID;
    };
}