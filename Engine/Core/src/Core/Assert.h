#pragma once

#include <cstdio>
#include <cstdlib>
#include <format>
#include <string>
#include <utility>

namespace axiom::detail
{
    // Called only once an assert has failed, so a passing assert never formats anything.
    template<typename... Args>
    [[noreturn]] void AssertFailed(const char* condition, const char* file, int line, std::format_string<Args...> fmt, Args&&... args)
    {
        const std::string message = std::format(fmt, std::forward<Args>(args)...);
        std::fprintf(stderr, "Assertion failed: %s\n  %s\n  %s:%d\n", condition, message.c_str(), file, line);
        std::abort();
    }
}

/// Stops the program when a condition a programmer guarantees doesn't hold. Bad input from a user or a file is
/// logged instead. The message is a std::format string, checked at compile time, and is only formatted on failure.
///
/// @code{.cpp}
/// AX_ASSERT(view.width != 0, "Invalid view render target dimension!");
/// AX_ASSERT(factory, "Reflected type '{}' has no factory", name);
/// @endcode
///
/// The message must be a string literal; a runtime string goes in as an argument: AX_ASSERT(ok, "{}", text).
/// Release builds abort without a message and don't evaluate the message arguments.
#ifdef AX_DEBUG
    #define AX_ASSERT(condition, ...) \
        do { if (!(condition)) { \
            ::axiom::detail::AssertFailed(#condition, __FILE__, __LINE__, __VA_ARGS__); \
        }} while(0)
#else
    #define AX_ASSERT(condition, ...) \
        do { if (!(condition)) { std::abort(); } } while(0)
#endif
