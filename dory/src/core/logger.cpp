#include "core/logger.h"

namespace DORY
{
    void AssertionFailure(const char* expression, const char* message, const char* file, signed int line)
    {
        log(COLOR_FATAL, "[FATAL]\x1b[0m \t", "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }
} // namespace DORY