#include "Scene/SceneModule.h"
#include "SceneModule.h"
namespace axiom
{
    SceneModule::SceneModule(Application& application)
    : ApplicationModule(application)
    {
        m_activeScene = MakeUnique<Scene>("Scene");
    }

    Scene* SceneModule::GetActiveScene() const
    {
        return m_activeScene.get();
    }

    bool SceneModule::HasActiveScene() const
    {
        return m_activeScene != nullptr;
    }

    void SceneModule::OnUpdate(float deltaTime)
    {
        m_activeScene->OnUpdate(deltaTime);
    }

    void SceneModule::OnBeginFrame()
    {
        m_activeScene->OnBeginFrame();
    }

    void SceneModule::OnRender()
    {
        m_activeScene->OnRender();
    }

    void SceneModule::OnEndFrame()
    {
        m_activeScene->OnEndFrame();
    }
}
