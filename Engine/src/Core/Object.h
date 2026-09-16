#pragma once
#include <random>
#include "Core/Types.h"

namespace axiom
{
    class Archive;

    class Object
    {
        friend class EngineReflection;

    public:
        Object();
        const String& GetID() const { return m_id; }

        virtual void Deserialize(Archive& ar);
        virtual void Serialize(Archive& ar);
    
    protected:
        friend class SceneLoader;
        String m_id;
        String GenerateUUID();
    };    
} // namespace axiom
