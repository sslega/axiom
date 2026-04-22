#pragma once

#include "Core/Types.h"
#include "Core/Application.h"
#include <format>

namespace axiom
{
    class Log
    {
    public:
        template<typename... Args>
        static void Info(const char* fmt, Args&&... args)
        {
            Application::Get().GetLog().InfoInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

        template<typename... Args>
        static void Error(const char* fmt, Args&&... args)
        {
            Application::Get().GetLog().ErrorInternal(std::vformat(fmt, std::make_format_args(args...)));
        }

    protected:
        virtual void InfoInternal(StringView message) = 0;
        virtual void ErrorInternal(StringView message) = 0;
    };

    class ConsoleLog : public Log
    {
        virtual void InfoInternal(StringView message) override
        {
            printf("[INFO] %.*s\n", (int)message.size(), message.data());
        }

        virtual void ErrorInternal(StringView message) override
        {
            printf("[ERROR] %.*s\n", (int)message.size(), message.data());
        }
    };
}