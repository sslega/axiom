#pragma once

#include "Core/ApplicationSubsystem.h"

namespace axiom
{
    class ImGuiSubsystem : public ApplicationSubsystem
    {
    public:
        ImGuiSubsystem(Application& application);

    protected:
        void OnInitialize() override;
        void OnShutdown() override;
        void OnBeginFrame() override;
        void OnRender() override;
        void OnEndFrame() override;
    };
}
