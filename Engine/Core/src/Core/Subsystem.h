#pragma once

namespace axiom
{
    class Subsystem
    {
    protected:
        virtual void OnRegister() {}
        virtual void OnUnregister() {}
        virtual void OnInitialize() {}
        virtual void OnShutdown() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender() {}
        virtual void OnEndFrame() {}
    };
}