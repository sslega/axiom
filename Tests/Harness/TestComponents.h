#pragma once
#include "Scene/Component.h"
#include "Reflection/Reflection.h"

namespace axiom::tests
{
    class ReqLeaf : public Component
    {
    };

    class ReqMiddle: public Component
    {
    public:
        const virtual Vector<std::type_index>& GetRequiredComponents() const override;
    };

    class ReqTop : public Component
    {
    public:
        const virtual Vector<std::type_index>& GetRequiredComponents() const override;
    };

    void RegisterTestComponents(TypeRegistry& registry);
}