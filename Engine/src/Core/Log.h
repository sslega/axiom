#pragma once

#include "Core/Types.h"
#include <format>

namespace axiom
{
    class Log
    {
    public:

        template<typename... Args>
        static void Info(const char* fmt, Args&&... args)
        {
            s_instance->InfoInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

        template<typename... Args>
        static void Error(const char* fmt, Args&&... args)
        {
            s_instance->ErrorInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

    protected:
        virtual void InfoInternal(StringView message) = 0;
        virtual void ErrorInternal(StringView message) = 0;

    private:
        static void SetInstance(Log* instance) { s_instance = instance; }
        inline static Log* s_instance = nullptr;

        friend class Application;
    };

    class ConsoleLog : public Log
    {
        virtual void InfoInternal(StringView message) override
        {
            printf("[INFO] %.*s\n", (int)message.size(), message.data());
            fflush(stdout);
        }

        virtual void ErrorInternal(StringView message) override
        {
            printf("[ERROR] %.*s\n", (int)message.size(), message.data());
            fflush(stdout);
        }
    };
}