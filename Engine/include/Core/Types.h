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

    template<typename V>
    using TypeMap       = std::unordered_map<std::type_index, V>;

    template<typename V>
    using StringMap       = std::unordered_map<String, V>;

    template<typename T>
    constexpr std::type_index TypeID() { return std::type_index(typeid(T));};

}