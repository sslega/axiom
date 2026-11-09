#pragma once

#include "Core/ApplicationSubsystem.h"

namespace axiom
{
    class ApplicationWindow;

    class ImGuiSubsystem : public ApplicationSubsystem
    {
    public:
        ImGuiSubsystem(SubsystemHost& host, ApplicationWindow& window);

    protected:
        void OnInitialize() override;
        void OnShutdown() override;
        void OnBeginFrame() override;
        void OnRender() override;
        void OnEndFrame() override;

    private:
        ApplicationWindow& m_window;
    };
}
