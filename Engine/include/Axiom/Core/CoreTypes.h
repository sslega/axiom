#pragma once

#include <string>
#include <iostream>

namespace axiom
{
    using String        = std::string;
    using StringView    = std::string_view;
    using uint32        = std::uint32_t;

    template<typename T>
    using UniquePtr     = std::unique_ptr<T>;
}