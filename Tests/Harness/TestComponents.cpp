#include "TestComponents.h"

namespace axiom::tests
{
    
    const Vector<std::type_index> &ReqMiddle::GetRequiredComponents() const
    {
        static const Vector<std::type_index> required = { TypeID<ReqLeaf>() };
        return required;
    }
    
    const Vector<std::type_index> &ReqTop::GetRequiredComponents() const
    {
        static const Vector<std::type_index> required = { TypeID<ReqMiddle>() };
        return required;
    }

    void RegisterTestComponents(TypeRegistry& registry)
    {
        {
            TypeDescriptor desc;
            desc.name = "ReqLeaf";
            desc.type = typeid(ReqLeaf);
            desc.baseType = typeid(Component);
            desc.factory = [] { return MakeUnique<ReqLeaf>(); };
            registry.Register(std::move(desc));
        }
        
        {
            TypeDescriptor desc;
            desc.name = "ReqMiddle";
            desc.type = typeid(ReqMiddle);
            desc.baseType = typeid(Component);
            desc.factory = [] { return MakeUnique<ReqMiddle>(); };
            registry.Register(std::move(desc));
        }

        {
            TypeDescriptor desc;
            desc.name = "ReqTop";
            desc.type = typeid(ReqTop);
            desc.baseType = typeid(Component);
            desc.factory = [] { return MakeUnique<ReqTop>(); };
            registry.Register(std::move(desc));
        }
    }

}