#pragma once

#include "Core/Types.h"
#include <format>

namespace axiom
{
    class Log
    {
    public:

        // A single argument is logged verbatim, so braces in it are safe.
        static void Info(StringView message)
        {
            s_instance->InfoInternal(message);
        }

        // Needs at least one argument: an empty pack would win overload resolution for string literals.
        template<typename... Args> requires (sizeof...(Args) > 0)
        static void Info(const char* fmt, Args&&... args)
        {
            s_instance->InfoInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

        static void Warning(StringView message)
        {
            ++s_warningCount;
            s_instance->WarningInternal(message);
        }

        template<typename... Args> requires (sizeof...(Args) > 0)
        static void Warning(const char* fmt, Args&&... args)
        {
            ++s_warningCount;
            s_instance->WarningInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

        static void Error(StringView message)
        {
            ++s_errorCount;
            s_instance->ErrorInternal(message);
        }

        template<typename... Args> requires (sizeof...(Args) > 0)
        static void Error(const char* fmt, Args&&... args)
        {
            ++s_errorCount;
            s_instance->ErrorInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

        /// How many errors have been logged since the program started.
        static uint32 GetErrorCount() { return s_errorCount; }
        static uint32 GetWarningCount() { return s_warningCount; }

    protected:
        virtual void InfoInternal(StringView message) = 0;
        virtual void WarningInternal(StringView message) = 0;
        virtual void ErrorInternal(StringView message) = 0;

    private:
        static void SetInstance(Log* instance) { s_instance = instance; }
        inline static Log* s_instance = nullptr;
        inline static uint32 s_errorCount = 0;
        inline static uint32 s_warningCount = 0;

        friend class Application;
    };

    class ConsoleLog : public Log
    {
        virtual void InfoInternal(StringView message) override
        {
            printf("[INFO] %.*s\n", (int)message.size(), message.data());
            fflush(stdout);
        }

        virtual void WarningInternal(StringView message) override
        {
            printf("[WARNING] %.*s\n", (int)message.size(), message.data());
            fflush(stdout);
        }

        virtual void ErrorInternal(StringView message) override
        {
            printf("[ERROR] %.*s\n", (int)message.size(), message.data());
            fflush(stdout);
        }
    };
}