#pragma once

#include "Core/ApplicationModule.h"

namespace axiom
{
    class ImGuiModule : public ApplicationModule
    {
    public:
        ImGuiModule(Application& application);

    protected:
        void OnInitialize()  override;
        void OnShutdown()    override;
        void OnBeginFrame()  override;
        void OnEndFrame()    override;
    };
}
