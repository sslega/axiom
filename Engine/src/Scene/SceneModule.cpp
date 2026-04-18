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
}

