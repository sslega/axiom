#include "Scene/SceneModule.h"
#include "SceneModule.h"
#include "Core/Assert.h"

namespace axiom
{
    SceneModule::SceneModule(Application& application)
    : ApplicationModule(application)
    {
        m_activeScene = MakeUnique<Scene>("Scene");
    }

    Scene& SceneModule::GetActiveScene() const
    {
        AX_ASSERT(m_activeScene, "Scene cannot be null!");
        return *m_activeScene.get();
    }

    void SceneModule::OnRegister()
    {
        m_activeScene->Register();
    }

    void SceneModule::OnInitialize()
    {
        m_activeScene->Initialize();
    }

    void SceneModule::OnShutdown()
    {
        m_activeScene->Shutdown();
    }

    void SceneModule::OnUpdate(float deltaTime)
    {
        m_activeScene->Update(deltaTime);
    }

    void SceneModule::OnBeginFrame()
    {
        m_activeScene->BeginFrame();
    }

    void SceneModule::OnRender()
    {
        m_activeScene->Render();
    }

    void SceneModule::OnEndFrame()
    {
        m_activeScene->EndFrame();
    }
}
