#pragma once
#include <random>
#include "Core/Types.h"

namespace axiom
{
    class Archive;

    class Object
    {
    public:
        Object();
        const String& GetID() const { return m_id; }
        const String& GetName() const { return m_name; }
        void SetName(String& name) { m_name = name; }

        virtual void Deserialize(Archive& ar);
        virtual void Serialize(Archive& ar);
    
    protected:
        friend class SceneLoader;
        String m_id;
        String m_name;
        String GenerateUUID();
    };    
} // namespace axiom
