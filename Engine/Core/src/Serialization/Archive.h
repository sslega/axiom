#pragma once

#include "Core/Types.h"
#include "nlohmann/json.hpp"
#include "Resources/IResourceResolver.h"
#include "Resources/ResourceRef.h"
#include "Math/Vector.h"

namespace axiom
{
    /// Reads and writes named values on one JSON node. Make one Archive per node.
    ///
    /// Writing the same key twice overwrites it. Read() skips a missing key and leaves the value
    /// unchanged, so a typo in a key name fails silently.
    ///
    /// @code{.cpp}
    /// nlohmann::json node;
    /// Archive ar(node, resourceResolver);
    /// ar.Write("name", name); // node["name"] = name
    /// ar.Read("name", name);
    /// entity->Serialize(ar);  // writes the entity's own fields into node
    /// @endcode
    ///
    /// Entity and Component only handle their own fields. The caller builds the `entities` and `components`
    /// arrays and the component `type` key; SceneLoader shows the file's shape.
    ///
    /// New value type: add a Serializer<T> specialization in this file.
    class Archive
    {
    public:
        Archive(nlohmann::json& node, IResourceResolver& resourceResolver)
        : m_node(node)
        , m_resourceResolver(resourceResolver)
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
            ref.Resolve(m_resourceResolver);
        }

        template<typename T> friend struct Serializer;
    private:
        nlohmann::json& m_node;
        IResourceResolver& m_resourceResolver;
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

    template<>
    struct Serializer<Color>
    {
        static void Write(Archive& ar, const String& key, const Color& value)
        {
            ar.m_node[key] = { value.x, value.y, value.z };
        }
        static void Read(Archive& ar, const String& key, Color& value)
        {
            auto& arr = ar.m_node[key];
            value = { arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>() };
        }
    };
    
} // namespace axiom
