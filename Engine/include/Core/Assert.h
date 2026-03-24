#pragma once

#ifdef AX_DEBUG
    #define AX_ASSERT(condition, msg) \
        do { if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s\n  %s\n  %s:%d\n", \
                #condition, msg, __FILE__, __LINE__); \
            std::abort(); \
        }} while(0)
    #define AX_FATAL(msg) \
        do { \
            fprintf(stderr, "Fatal error: %s\n  %s:%d\n", msg, __FILE__, __LINE__); \
            std::abort(); \
        } while(0)
#else
    #define AX_ASSERT(condition, msg) \
        do { if (!(condition)) { std::abort(); } } while(0)
    #define AX_FATAL(msg) \
        do { std::abort(); } while(0)
#endif