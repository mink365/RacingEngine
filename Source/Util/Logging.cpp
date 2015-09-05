#include "Logging.h"

#ifdef RE_PLATFORM_ANDROID
#include <android/log.h>
#endif

namespace re {

#ifndef RE_PLATFORM_ANDROID
constexpr FILE* LogLevelToFile(LogLevel level)
{
    switch(level)
    {
    case LogLevel::Verbose:
        return stdout;
    case LogLevel::Debug:
        return stdout;
    case LogLevel::Info:
        return stdout;
    case LogLevel::Warn:
        return stdout;
    case LogLevel::Error:
        return stderr;
    }
}

constexpr const char* LogLevelToTag(LogLevel level)
{
    switch(level)
    {
    case LogLevel::Verbose:
        return "[V]";
    case LogLevel::Debug:
        return "[D]";
    case LogLevel::Info:
        return "[I]";
    case LogLevel::Warn:
        return "[W]";
    case LogLevel::Error:
        return "[E]";
    }
}

#else
constexpr int LogLevelToPrio(LogLevel level)
{
    switch(level)
    {
    case LogLevel::Verbose:
        return ANDROID_LOG_VERBOSE;
    case LogLevel::Debug:
        return ANDROID_LOG_DEBUG;
    case LogLevel::Info:
        return ANDROID_LOG_INFO;
    case LogLevel::Warn:
        return ANDROID_LOG_WARN;
    case LogLevel::Error:
        return ANDROID_LOG_ERROR;
    }
}
#endif

const char* MODULE_LOG_TAG = "RacingEngine";

void Log(LogLevel level, char const* format, fmt::ArgList args)
{
#ifdef RE_PLATFORM_ANDROID
    int prio = LogLevelToPrio(level);
    std::string s = fmt::format(format, args);
    __android_log_print(prio, MODULE_LOG_TAG, "[%s]", s.c_str());
#else
    FILE* out_file = LogLevelToFile(level);
    const char* level_tag = LogLevelToTag(level);
    fmt::print(out_file, "{0} [{1}] ", level_tag, MODULE_LOG_TAG);
    fmt::print(out_file, format, args);
    fmt::print(out_file, "\n");
    fflush(out_file);
#endif
}

void Log(char const* format, fmt::ArgList args)
{
    Log(LogLevel::Debug, format, args);
}

void LogWarning(char const* format, fmt::ArgList args)
{
    Log(LogLevel::Warn, format, args);
}

void LogError(char const* format, fmt::ArgList args)
{
    Log(LogLevel::Error, format, args);
}

}

