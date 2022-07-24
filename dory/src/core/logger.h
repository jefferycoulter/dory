#ifndef DORY_LOGGER_INCL
#define DORY_LOGGER_INCL

#include "core/core.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

#define __STDC_WANT_LIB_EXT1__ 1 // needed for localtime_s

namespace DORY
{
    #define LOG_WARN_ENABLED 1
    #define LOG_INFO_ENABLED 1
    #define LOG_DEBUG_ENABLED 1
    #define LOG_TRACE_ENABLED 1

    // disable debug and trace logging for release
    #if DRELEASE == 1
    #define LOG_DEBUG_ENABLED 0
    #define LOG_TRACE_ENABLED 0
    #endif

    #define COLOR_TRACE "" // black
    #define COLOR_DEBUG "\x1b[33m" // yellow
    #define COLOR_INFO "\x1b[32m" // green
    #define COLOR_WARN "\x1b[34m" // blue
    #define COLOR_ERROR "\x1b[31m" // red
    #define COLOR_FATAL "\x1b[35m" // magenta

    #ifdef __APPLE__
        /**
         * @brief log a message to the console. this is used in the macros defined below
         * @tparam Args the types of the variadic arguments to the log message
         * @param color the color of the text to be printed
         * @param priority_name the name of the log priority, e.g. "FATAL", "ERROR", "WARN", etc.
         * @param msg the message to be printed
         * @param args variadic arguments to the log message
         */
        template<typename ... Args>
        static void log(const char* color, const char* priority_name, const char* msg, Args... args)
        {   
            time_t t = time(0);
            struct tm buffer;
            char time_str[26];
            asctime_r(localtime_r(&t, &buffer), time_str);

            printf("%s%s%s", color, priority_name, time_str);
            //printf("%s", color);
            printf(msg, args...);
            printf("\n");

            // https://stackoverflow.com/questions/27375089/what-is-the-easiest-way-to-print-a-variadic-parameter-pack-using-stdostream
            //std::stringstream ss;
            //((ss << std::forward<Args>(args)), ...);
            //std::cout << color << priority_name << " " << color << time_str << " " << msg << ss.str() << "\n";
            //std::cout << "\n";
        }

    #elif _WIN64
        #define _CRT_SECURE_NO_WARNINGS
        #pragma warning(disable:4996)
        #include <windows.h> // WinAPI header for changing color of console text
        template<typename ... Args>
        static void log(const char* color, const char* priority_name, const char* msg, Args... args)
        {
            time_t t = time(0);
            printf("%s%s%s", color, priority_name, asctime(localtime(&t)));
            printf(msg, args...);
            printf("\n");
        }
    #endif

    #define DFATAL(msg, ...) log(COLOR_FATAL, "[FATAL]\x1b[0m \t", msg, ##__VA_ARGS__)
    #define DERROR(msg, ...) log(COLOR_ERROR, "[ERROR]\x1b[0m \t", msg, ##__VA_ARGS__)
    #if LOG_WARN_ENABLED == 1
        #define DWARN(msg, ...) log(COLOR_WARN, "[WARN]\x1b[0m \t", msg, ##__VA_ARGS__)
    #else
        #define DWARN(msg, ...)
    #endif
    #if LOG_INFO_ENABLED == 1
        #define DINFO(msg, ...) log(COLOR_INFO, "[INFO]\x1b[0m \t", msg, ##__VA_ARGS__)
    #else
        #define DINFO(msg, ...)
    #endif
    #if LOG_DEBUG_ENABLED == 1
        #define DDEBUG(msg, ...) log(COLOR_DEBUG, "[DEBUG]\x1b[0m \t", msg, ##__VA_ARGS__)
    #else
        #define DDEBUG(msg, ...)
    #endif
    #if LOG_TRACE_ENABLED == 1
        #define DTRACE(msg, ...) log(COLOR_TRACE, "[TRACE] \t", msg, ##__VA_ARGS__)
    #else
        #define DTRACE(msg, ...)
    #endif
} // namespace DORY

#endif // DORY_LOGGER_INCL