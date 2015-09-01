#ifndef LOGGING
#define LOGGING

#include "External/cppformat/format.h"

namespace re {

enum class LogLevel
{
    Verbose,
    Debug,
    Info,
    Warn,
    Error,
};

void Log(LogLevel level, char const* format, fmt::ArgList args);
void Log(char const* format, fmt::ArgList args);
void LogWarning(char const* format, fmt::ArgList args);
void LogError(char const* format, fmt::ArgList args);

FMT_VARIADIC(void, Log, LogLevel, const char *)
FMT_VARIADIC(void, Log, const char *)
FMT_VARIADIC(void, LogWarning, const char *)
FMT_VARIADIC(void, LogError, const char *)

}

#endif // LOGGING

