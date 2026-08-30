#pragma once
#include "Core/Types.h"
#include <imgui.h>

namespace axiom
{
    class EditorPanel
    {
    public:
        virtual ~EditorPanel() = default;

        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() = 0;
        virtual const String& GetName() const = 0;
    };
}