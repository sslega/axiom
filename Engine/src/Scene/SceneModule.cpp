#include "Scene/SceneModule.h"
namespace axiom
{
    SceneModule::SceneModule(Application& application)
    : ApplicationModule(application)
    {
    }

    Scene* SceneModule::CreateScene(const String &name)
    {
        m_activeScene = MakeUnique<Scene>(name);
        return m_activeScene.get();
    }

    Scene* SceneModule::GetActiveScene() const
    {
        return m_activeScene.get();
    }

    bool SceneModule::HasActiveScene() const
    {
        return m_activeScene != nullptr;
    }
}