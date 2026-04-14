#pragma once

#include "Memory.h"

#include <string>
#include <string_view>
#include <cstdint>
#include <utility>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <typeindex>
#include <unordered_set>

namespace axiom
{
    using String        = std::string;
    using StringView    = std::string_view;
    
    using Path          = std::filesystem::path;
    
    using uint32        = std::uint32_t;
    using uint16        = std::uint16_t;
    using uint8         = std::uint8_t;

    template<typename T>
    using Vector        = std::vector<T>;

    template<typename T>
    using UnorderedSet  = std::unordered_set<T>;

    template<typename K, typename V>
    using UnorderedMap  = std::unordered_map<K, V>;

    template<typename V>
    using TypeMap       = std::unordered_map<std::type_index, V>;

    template<typename V>
    using StringMap       = std::unordered_map<String, V>;

    template<typename T>
    std::type_index TypeID() { return std::type_index(typeid(T));};

    template<typename T>
    using UniquePtr     = std::unique_ptr<T>;
    template<typename T, typename... Args>
    UniquePtr<T> MakeUnique(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    
    template<typename T>
    using SharedPtr     = std::shared_ptr<T>;
    template<typename T, typename... Args>
    SharedPtr<T> MakeShared(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}