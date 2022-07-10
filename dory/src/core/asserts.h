#ifndef DORY_ASSERTS_INCL
#define DORY_ASSERTS_INCL

#include "defines.h"

namespace DORY
{
    #define DASSERTIONS_ENABLED

    #ifdef DASSERTIONS_ENABLED
        #if defined(_MSC_VER) // windows OS
        #include <intrin.h>
            /** 
             * @brief causes a debug breakpoint to be hit
             */
            #define debug_break() __debugbreak()
        #elif defined(__APPLE__) // macOS
            #if defined(__x86_64__) // intel
                /** 
                 * @brief causes a debug breakpoint to be hit
                 */
                #define debug_break() __asm__("int $3")
            #elif defined(__arm64__) // arm64 mac
                /** 
                 * @brief causes a debug breakpoint to be hit
                 */
                #define debug_break() __asm__("brk #0")
            #else
                #error "Unsupported architecture"
            #endif
        #else // linux OS
            /** 
             * @brief causes a debug breakpoint to be hit
             */
            #define debug_break() __builtin_trap()
        #endif

        /**
         * @brief reports an assertion failure
         * @param expression the expression to be reported.
         * @param message the message to be printed with the assertion failure.
         * @param file the path and name of the code file containing the failure
         * @param line the line number in the file where the failure occurred
         */
        void assertion_failure(const char* expression, const char* message, const char* file, i32 line);

        /**
         * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
         * a break point
         * @param expr the expression to be evaluated.
         */
        #define DASSERT(expr)                                         \
            {                                                         \
                if (expr) {                                           \
                } else {                                              \
                    assertion_failure(#expr, "", __FILE__, __LINE__); \
                    debug_break();                                    \
                }                                                     \
            }

        /**
         * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
         * a break point
         * @param expr the expression to be evaluated.
         * @param message the message to be reported with the failure
         */
        #define DASSERT_MSG(expr, message)                                 \
            {                                                              \
                if (expr) {                                                \
                } else {                                                   \
                    assertion_failure(#expr, message, __FILE__, __LINE__); \
                    debug_break();                                         \
                }                                                          \
            }

        #if defined(_DEBUG) || defined(DEBUG)
            /**
             * @brief asserts the provided expression to be true, and logs a failure if not. then triggers
             * a break point
             * @param expr the expression to be evaluated.
             */
            #define DASSERT_DEBUG(expr)                                   \
                {                                                         \
                    if (expr) {                                           \
                    } else {                                              \
                        assertion_failure(#expr, "", __FILE__, __LINE__); \
                        debug_break();                                    \
                    }                                                     \
                }
        #else // not debug mode
            #define KASSERT_DEBUG(expr)
        #endif
    #else // assertions disabled
        #define KASSERT(expr)
        #define KASSERT_MSG(expr, message) 
        #define KASSERT_DEBUG(expr) 

    #endif

} // namespace DORY

#endif // DORY_ASSERTS_INCL