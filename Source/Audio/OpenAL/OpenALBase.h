/*****************************************************************************

 Copyright (c) 2015 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef OPENALBASE_H
#define OPENALBASE_H

#include <AL/al.h>
#include "Util/Logging.h"

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = NULL; \
    }

/**
 * Executes the specified AL code and checks the AL error afterwards
 * to ensure it succeeded.
 *
 * The AL_LAST_ERROR macro can be used afterwards to check whether a AL error was
 * encountered executing the specified code.
 */
#define AL_CHECK( al_code ) do \
    { \
        while (alGetError() != AL_NO_ERROR) ; \
        al_code; \
        __al_error_code = alGetError(); \
        if (__al_error_code != AL_NO_ERROR) \
        { \
            LogError(#al_code ": {}", (int)__al_error_code); \
        } \
    } while(0)

/** Global variable to hold AL errors
 * @script{ignore} */
extern ALenum __al_error_code;

/**
 * Accesses the most recently set global AL error.
 */
#define AL_LAST_ERROR() __al_error_code

#endif // OPENALBASE_H

