#pragma once
#include "Renderer/PostProcessBase.h"
#include "Math/Vector.h"

namespace axiom
{
    class FrameBuffer;
    class Shader;

    class OutlinePostProcess : public PostProcessBase
    {
    public:
        explicit OutlinePostProcess(const Vec4& outlineColor);

        virtual void Execute(const PostProcessContext& context) override;
    
    private:
        Vec4 m_outlineColor;
        SharedPtr<Shader> m_shader;
        SharedPtr<Shader> m_JFAInitShader;
        SharedPtr<Shader> m_JFAStepShader;
    };
}