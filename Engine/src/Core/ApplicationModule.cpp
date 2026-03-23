#include "Core/ApplicationModule.h"
#include <cstdio>
#include <typeinfo>

namespace axiom
{
    ApplicationModule::ApplicationModule(Application& application)
        : m_application(application)
    {
    }

    bool ApplicationModule::Initialize()
    {
        return true;
    }

    void ApplicationModule::OnRegister()
    {
        printf("Module registered: %s\n", typeid(*this).name());
    }

    void ApplicationModule::OnUnregister()
    {
    }

    void ApplicationModule::Shutdown()
    {
    }

    void ApplicationModule::Update()
    {
    }

    void ApplicationModule::Render()
    {
    }

    Application& ApplicationModule::GetApp() const
    {
        return m_application;
    }

}

