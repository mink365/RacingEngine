#include "StringUtil.h"

#include <stdarg.h>
#include <stdlib.h>

#define STARTING_BUFFER_SIZE  512

StringUtil::StringUtil()
{
}

#define GET_VARARGS_RESULT(buffer, msgSize, len, result, pszFormat) \
{ \
    va_list ap; \
    va_start(ap, pszFormat); \
    result = vsnprintf(buffer, len, pszFormat, ap); \
    va_end(ap); \
    \
    if (result > msgSize) { \
        result = -1; \
    } \
}

std::string StringUtil::Printf(const char *pszFormat, ...)
{
    int32_t		bufferSize	= STARTING_BUFFER_SIZE;
    char	startingBuffer[STARTING_BUFFER_SIZE];
    char*	buffer		= startingBuffer;
    int32_t		result		= -1;

    // First try to print to a stack allocated location
    GET_VARARGS_RESULT( buffer, bufferSize, bufferSize-1, result, pszFormat);

    // If that fails, start allocating regular memory
    if( result == -1 )
    {
        buffer = NULL;
        while(result == -1)
        {
            bufferSize *= 2;
            buffer = (char*) realloc( buffer, bufferSize * sizeof(char) );
            GET_VARARGS_RESULT( buffer, bufferSize, bufferSize-1, result, pszFormat);
        };
    }

    buffer[result] = 0;

    std::string ResultString(buffer);

    if( bufferSize != STARTING_BUFFER_SIZE )
    {
        free( buffer );
    }

    return ResultString;
}
