#pragma once
#include "Renderer/PostProcessBase.h"

namespace axiom
{
    class FrameBuffer;
    class Shader;

    class PostProcessTest : public PostProcessBase
    {
    public:
        virtual void Execute(RenderSubsystem& renderer, FrameBuffer& source, FrameBuffer& target) override;
    
    private:
        SharedPtr<Shader> m_shader;
    };
}