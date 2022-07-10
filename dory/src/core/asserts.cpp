#include "asserts.h"
#include "logger.h"

namespace DORY
{
    void assertion_failure(const char* expression, const char* message, const char* file, i32 line)
    {
        log(COLOR_FATAL, "[FATAL]\x1b[0m \t", "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }
} // namespace DORY