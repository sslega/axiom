#pragma once

#include "Core/Types.h"
#include "nlohmann/json.hpp"
#include "Resources/ResourceSubsystem.h"
#include "Math/Vector.h"

namespace axiom
{
    class Archive
    {
    public:
        Archive(nlohmann::json& node, ResourceSubsystem& resourceSubsystem)
        : m_node(node)
        , m_resourceSubsystem(resourceSubsystem)
        {
        }

        template<typename T>
        void Write(const String& key, const T& value)
        {
            Serializer<T>::Write(*this, key, value);
        }

        template<typename T>
        void Read(const String& key, T& value)
        {
            if(m_node.contains(key))
            {
                Serializer<T>::Read(*this, key, value);
            }
        }

        template<typename T>
        void ResolveRef(ResourceRef<T>& ref)
        {
            m_resourceSubsystem.ResolveRef(ref);
        }

        template<typename T> friend struct Serializer;
    private:
        nlohmann::json& m_node;
        ResourceSubsystem& m_resourceSubsystem;
    };

    template<typename T>
    struct Serializer;

    template<>
    struct Serializer<float>
    {
        static void Write(Archive& ar, const String& key, const float& value) { ar.m_node[key] = value; }
        static void Read(Archive& ar, const String& key, float& value) { value = ar.m_node[key].get<float>(); }
    };

    template<>
    struct Serializer<int>
    {
        static void Write(Archive& ar, const String& key, const int& value) { ar.m_node[key] = value; }
        static void Read(Archive& ar, const String& key, int& value){ value = ar.m_node[key].get<int>(); }
    };

    template<>
    struct Serializer<bool>
    {
        static void Write(Archive& ar, const String& key, const bool& value) { ar.m_node[key] = value; }
        static void Read(Archive& ar, const String& key, bool& value){ value = ar.m_node[key].get<bool>(); }
    };

    template<>
    struct Serializer<String>
    {
        static void Write(Archive& ar, const String& key, const String& value) { ar.m_node[key] = value; }
        static void Read(Archive& ar, const String& key, String& value){ value = ar.m_node[key].get<String>(); }
    };

    template<typename T>
    struct Serializer<ResourceRef<T>>
    {
        static void Write(Archive& ar, const String& key, const ResourceRef<T>& value)
        {
            ar.m_node[key] = value.GetPath();
        }

        static void Read(Archive& ar, const String& key, ResourceRef<T>& value)
        {
            value = ResourceRef<T>(ar.m_node[key].get<String>());
            ar.ResolveRef(value);
        }
    };


    template<>
    struct Serializer<Vec2>
    {
        static void Write(Archive& ar, const String& key, const Vec2& value)
        {
            ar.m_node[key] = { value.x, value.y };
        }
        static void Read(Archive& ar, const String& key, Vec2& value)
        {
            auto& arr = ar.m_node[key];
            value = { arr[0].get<float>(), arr[1].get<float>()};
        }
    };

    template<>
    struct Serializer<Vec3>
    {
        static void Write(Archive& ar, const String& key, const Vec3& value)
        {
            ar.m_node[key] = { value.x, value.y, value.z };
        }
        static void Read(Archive& ar, const String& key, Vec3& value)
        {
            auto& arr = ar.m_node[key];
            value = { arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>() };
        }
    };

    template<>
    struct Serializer<Vec4>
    {
        static void Write(Archive& ar, const String& key, const Vec4& value)
        {
            ar.m_node[key] = { value.x, value.y, value.z, value.w };
        }
        static void Read(Archive& ar, const String& key, Vec4& value)
        {
            auto& arr = ar.m_node[key];
            value = { arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>(), arr[3].get<float>() };
        }
    };
} // namespace axiom
