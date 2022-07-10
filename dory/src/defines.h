#ifndef DORY_DEFINES_INCL
#define DORY_DEFINES_INCL


namespace DORY 
{
    // unsigned int types.

    /**
     * @brief unsigned 8-bit integer
     */
    typedef unsigned char u8;

    /**
     * @brief unsigned 16-bit integer
     */
    typedef unsigned short u16;

    /**
     * @brief unsigned 32-bit integer
     */
    typedef unsigned int u32;

    /** 
     * @brief unsigned 64-bit integer
     */
    typedef unsigned long long u64;

    // signed int types.

    /**
     * @brief signed 8-bit integer
     */
    typedef signed char i8;

    /**
     * @brief signed 16-bit integer
     */
    typedef signed short i16;

    /**
     * @brief signed 32-bit integer
     */
    typedef signed int i32;

    /**
     * @brief signed 64-bit integer
     */
    typedef signed long long i64;

    // floating point types

    /**
     * @brief 32-bit floating point
     */
    typedef float f32;

    /**
     * @brief 64-bit floating point
     */
    typedef double f64;

    // boolean types

    /**
     * @brief 32-bit boolean
     */
    typedef int b32;

    /**
     * @brief 8-bit boolean
     */
    typedef bool b8;

    /** 
     * @brief a range, for example, of memory
     */
    typedef struct range {
        /** 
         * @brief the offset in bytes
         */
        u64 offset;

        /** 
         * @brief the size in bytes
         */
        u64 size;
    } range;

    // define static assertion based on compiler
    #if defined(__clang__) || defined(__gcc__)
    /**
     * @brief static assertion 
     */
    #define STATIC_ASSERT _Static_assert
    #else
    /** 
     * @brief static assertion
     */
    #define STATIC_ASSERT static_assert
    #endif

    // ensure all types are of the correct size.
    STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
    STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
    STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
    STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
    STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
    STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
    STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
    STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
    STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
    STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

    // define true and false
    #define true 1
    #define false 0

    // platform detection
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    // windows OS
    #define DPLATFORM_WINDOWS 1
    #ifndef _WIN64
    #error "64-bit is required on Windows!"
    #endif
    #elif defined(__linux__) || defined(__gnu_linux__)
    // linux OS
    #define DPLATFORM_LINUX 1
    #elif defined(__unix__)
    // just in case the above doesn't catch it
    #define KPLATFORM_UNIX 1
    #elif defined(__APPLE__)
    // macOS
    #define DPLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #else
    #error "Unknown platform!"
    #endif

} // namespace DORY

#endif // DORY_DEFINES_INCL