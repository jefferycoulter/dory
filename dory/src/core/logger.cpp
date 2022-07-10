#include "logger.h"

namespace DORY
{
    void assertion_failure(const std::string& expression, const std::string& message, const std::string& file, signed int line)
    {
        log(COLOR_FATAL, "[FATAL]\x1b[0m \t", "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }
} // namespace DORY