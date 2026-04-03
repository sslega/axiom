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

        virtual void UploadUniformMat4(const String& name,const Matrix4& matrix) override;
    
    private:
        uint32 m_rendererID;
    };
}