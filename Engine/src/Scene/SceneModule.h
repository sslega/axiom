#pragma once
#include "Core/ApplicationModule.h"
#include "Core/Types.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace axiom
{
    class SceneModule: public ApplicationModule
    {
    public:
        SceneModule(Application& application);

        // Scene* CreateScene(const String& name);
        Scene* GetActiveScene() const;
        bool HasActiveScene() const;
    
    protected:
        virtual void OnUpdate(float deltaTime) override;
        virtual void OnBeginFrame() override;
        virtual void OnRender() override;
        virtual void OnEndFrame() override;

    private:
        UniquePtr<Scene>  m_activeScene;
    };
}