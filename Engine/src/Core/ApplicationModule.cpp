#include "Core/ApplicationModule.h"
#include "Core/Application.h"

namespace axiom
{
    ApplicationModule::ApplicationModule(Application& application)
        : m_application(application)
    {
    }

    void ApplicationModule::Initialize()
    {
        OnInitialize();
    }

    void ApplicationModule::Shutdown()
    {
        OnShutdown();
    }

    void ApplicationModule::OnRegister()
    {
    }

    void ApplicationModule::OnUnregister()
    {
    }
}
