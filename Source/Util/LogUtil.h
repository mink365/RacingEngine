//
//  LogTag.h
//
//  Created by zhouyan on 2012-12-18.
//  Updated by zhouyan on 2013-03-04.
//
//  Note: Put the macro at the beginning of your cpp file AFTER the #include lines:
//        #define MODULE_LOG_TAG "YourTagName"
//        then all logs outputted using LOG, LOG_V, LOG_D, LOG_I, LOG_W, LOG_E in this cpp file have the tag "YourTagName".
//        Furthermore, you can use LOG_V_T, LOG_D_T, LOG_I_T, LOG_W_T, LOG_E_T to specify tag for each log.
//
//  Caution: NEVER define MODULE_LOG_TAG in your .h files. Always do this in .cpp files. In .h files, use LOG_V_T series.
//

#ifndef __LOG_TAG_H__
#define __LOG_TAG_H__

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef MODULE_LOG_TAG
#define MODULE_LOG_TAG "RacingEngine"
#endif

#ifdef RE_PLATFORM_ANDROID
#include <android/log.h>
#define LOG_V_T(tag, fmt, ...) __android_log_print(ANDROID_LOG_VERBOSE, tag, fmt, ##__VA_ARGS__)
#define LOG_D_T(tag, fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, tag, fmt, ##__VA_ARGS__)
#define LOG_I_T(tag, fmt, ...) __android_log_print(ANDROID_LOG_INFO, tag, fmt, ##__VA_ARGS__)
#define LOG_W_T(tag, fmt, ...) __android_log_print(ANDROID_LOG_WARN, tag, fmt, ##__VA_ARGS__)
#define LOG_E_T(tag, fmt, ...) __android_log_print(ANDROID_LOG_ERROR, tag, fmt, ##__VA_ARGS__)
#else

#define MAX_LEN         (16*1024 + 1)

inline void Log__(const char * pszFormat, ...)
{
    char szBuf[MAX_LEN];

    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, MAX_LEN, pszFormat, ap);
    va_end(ap);

    // Strip any trailing newlines from log message.
    size_t len = strlen(szBuf);
    while (len && szBuf[len-1] == '\n')
    {
      szBuf[len-1] = '\0';
      len--;
    }

    fprintf(stderr, "[%s]\n",  szBuf);
}

#define LOG_V_T(tag, fmt, ...) Log__("[V] [" tag "] " fmt, ##__VA_ARGS__)
#define LOG_D_T(tag, fmt, ...) Log__("[D] [" tag "] " fmt, ##__VA_ARGS__)
#define LOG_I_T(tag, fmt, ...) Log__("[I] [" tag "] " fmt, ##__VA_ARGS__)
#define LOG_W_T(tag, fmt, ...) Log__("[W] [" tag "] " fmt, ##__VA_ARGS__)
#define LOG_E_T(tag, fmt, ...) Log__("[E] [" tag "] " fmt, ##__VA_ARGS__)
#endif

#define LOG(fmt, ...) LOG_V(fmt, __VA_ARGS__)

#define LOG_V(fmt, ...) LOG_V_T(MODULE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOG_D(fmt, ...) LOG_D_T(MODULE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOG_I(fmt, ...) LOG_I_T(MODULE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) LOG_W_T(MODULE_LOG_TAG, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) LOG_E_T(MODULE_LOG_TAG, fmt, ##__VA_ARGS__)

#endif  // __LOG_TAG_H__
