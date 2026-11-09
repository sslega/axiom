#pragma once
#include "Renderer/PostProcessBase.h"

namespace axiom
{
    class FrameBuffer;
    class Shader;

    class OutlinePostProcess : public PostProcessBase
    {
    public:
        virtual void Execute(const PostProcessContext& context) override;
    
    private:
        SharedPtr<Shader> m_shader;
        SharedPtr<Shader> m_JFAInitShader;
        SharedPtr<Shader> m_JFAStepShader;
    };
}