#include "Object.h"
#include "Serialization/Archive.h"

namespace axiom
{
    Object::Object()
    {
        m_id = GenerateUUID();
    }

    void Object::Deserialize(Archive &ar)
    {
        ar.Read("id", m_id);
        ar.Read("name", m_name);
    }

    void Object::Serialize(Archive &ar)
    {   
        ar.Write("id", m_id);
        ar.Write("name", m_name);
    }

    String Object::GenerateUUID()
    {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<uint32> dist(0, 0xFFFFFFFF);
        char buf[37];
        snprintf(buf, sizeof(buf), "%08x-%04x-%04x-%04x-%08x%04x",
            dist(rng), dist(rng) & 0xFFFF, (dist(rng) & 0x0FFF) | 0x4000,
            (dist(rng) & 0x3FFF) | 0x8000, dist(rng), dist(rng) & 0xFFFF);
        return buf;
    }

} // namespace axiom
