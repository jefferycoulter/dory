#ifndef DORY_CORE_INCL
#define DORY_CORE_INCL

#include "logger.h"
#include <string>

namespace DORY
{
    // platform detection
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        // windows OS
        #define DPLATFORM_WINDOWS 1
        #ifndef _WIN64
            #error "64-bit is required on Windows."
        #endif
    #elif defined(__linux__) || defined(__gnu_linux__)
        // linux OS
        #define DPLATFORM_LINUX 1
    #elif defined(__unix__)
        // just in case the above doesn't catch it
        #define DPLATFORM_UNIX 1
    #elif defined(__APPLE__)
        // macOS
        #define DPLATFORM_APPLE 1
        #include <TargetConditionals.h>
    #else
        #error "Unknown platform."
    #endif

    // assertions
    #define DASSERTIONS_ENABLED

    #ifdef DASSERTIONS_ENABLED
        #if defined(_MSC_VER) // windows OS
        #include <intrin.h>
            // causes a debug breakpoint to be hit
            #define debug_break() __debugbreak()
        #elif defined(__APPLE__) // macOS
            // https://stackoverflow.com/questions/44140778/resumable-assert-breakpoint-on-ios-like-debugbreak-with-ms-compiler
            #if defined(__x86_64__) // intel
                // causes a debug breakpoint to be hit
                #define debug_break() __asm__("int $3")
            #elif defined(__arm64__) // arm64 mac
                // causes a debug breakpoint to be hit
                #define debug_break() __asm__("brk #0")
            #else
                #error "Unsupported architecture"
            #endif
        #else // linux OS
            // causes a debug breakpoint to be hit
            #define debug_break() __builtin_trap()
        #endif

        /**
         * @brief reports an assertion failure. defined in logger.cpp
         * @param expression the expression to be reported.
         * @param message the message to be printed with the assertion failure.
         * @param file the path and name of the code file containing the failure
         * @param line the line number in the file where the failure occurred
         */
        void assertion_failure(const std::string& expression, const std::string& message, const std::string& file, signed int line);

        /**
         * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
         * a break point
         * @param expr the expression to be evaluated.
         */
        #define DASSERT(expr) { if (expr) {} else { assertion_failure(#expr, "", __FILE__, __LINE__); debug_break(); } }

        /**
         * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
         * a break point
         * @param expr the expression to be evaluated.
         * @param message the message to be reported with the failure
         */
        #define DASSERT_MSG(expr, message) { if (expr) {} else { assertion_failure(#expr, message, __FILE__, __LINE__); debug_break(); } }

        #if defined(_DEBUG) || defined(DEBUG)
            /**
             * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
             * a break point
             * @param expr the expression to be evaluated.
             */
            #define DASSERT_DEBUG(expr) { if (expr) {} else { assertion_failure(#expr, "", __FILE__, __LINE__); debug_break(); } }
        #else // not debug mode
            #define KASSERT_DEBUG(expr)
        #endif
    #else // assertions disabled
        #define KASSERT(expr)
        #define KASSERT_MSG(expr, message) 
        #define KASSERT_DEBUG(expr) 

    #endif

    // define static assertion based on compiler
    #if defined(__clang__) || defined(__gcc__)
        #define STATIC_ASSERT _Static_assert
    #else
        #define STATIC_ASSERT static_assert
    #endif

    // ensure all types are of the correct size.
    STATIC_ASSERT(sizeof(unsigned char) == 1, "Expected unsigned char to be 1 byte.");
    STATIC_ASSERT(sizeof(unsigned short) == 2, "Expected unsigned short to be 2 bytes.");
    STATIC_ASSERT(sizeof(unsigned int) == 4, "Expected unsigned int to be 4 bytes.");
    STATIC_ASSERT(sizeof(unsigned long long) == 8, "Expected unsigned long long to be 8 bytes.");
    STATIC_ASSERT(sizeof(signed char) == 1, "Expected signed char to be 1 byte.");
    STATIC_ASSERT(sizeof(signed short) == 2, "Expected signed short to be 2 bytes.");
    STATIC_ASSERT(sizeof(signed int) == 4, "Expected signed int to be 4 bytes.");
    STATIC_ASSERT(sizeof(signed long long) == 8, "Expected signed long long to be 8 bytes.");
    STATIC_ASSERT(sizeof(float) == 4, "Expected float to be 4 bytes.");
    STATIC_ASSERT(sizeof(double) == 8, "Expected double to be 8 bytes.");

} // namespace DORY

#endif // DORY_CORE_INCL