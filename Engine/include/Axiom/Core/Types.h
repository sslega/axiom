#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <utility>
#include <vector>

namespace axiom
{
    using String        = std::string;
    using StringView    = std::string_view;
    using uint32        = std::uint32_t;
    using uint16        = std::uint16_t;
    using uint8         = std::uint8_t;

    template<typename T>
    using Vector     = std::vector<T>;
}