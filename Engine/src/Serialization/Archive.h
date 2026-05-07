#pragma once

#include "Core/Types.h"
#include "nlohmann/json.hpp"
#include "Resources/ResourceHandle.h"
#include "Math/Vector.h"

namespace axiom
{
    class ResourceModule;

    class Archive
    {
    public:

        Archive(nlohmann::json& node, Vector<UniquePtr<IResolvable>>& handles);

        template<typename T>
        void Write(const String& key, T& value)
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

        void RegisterHandle(UniquePtr<IResolvable> handle);
        void MergeHandlesInto(Archive& other);
        
        template<typename T> friend struct Serializer;
    private:
        nlohmann::json& m_node;
        Vector<UniquePtr<IResolvable>>& m_handles;
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
    struct Serializer<SharedPtr<T>>
    {
        static void Write(Archive& ar, const String& key, const SharedPtr<T>& value)
        {
            ar.m_node[key] = value->GetPath();
        }

        static void Read(Archive& ar, const String& key, SharedPtr<T>& value)
        {
            String path = ar.m_node[key].get<String>();
            ar.RegisterHandle(MakeUnique<ResourceHandle<T>>(path, value));
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
