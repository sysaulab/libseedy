#ifndef _SEEDY_H
#define _SEEDY_H


/*****************************************************************************
 *                                                                           *
 *   stdint.h STANDARD INT SUPPORT AND SHIMS.                                *
 *                                                                           *
 *****************************************************************************/


/*
 *    Turbo C for DOS ...
 */

#if defined(__TURBOC__)

    typedef unsigned char  uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned long  uint32_t;
    typedef char  int8_t;
    typedef short int16_t;
    typedef long  int32_t;

/*
 *    MISCROSOFT COMPILERS EXCEPTIONS APPLIED FIRST, THEN C99 IS ASSUMED.
 */
#elif defined(_MSC_VER) /* MICROSOFT COMPILER */

    /* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING WIN16 TARGET */
#   if (_MSC_VER < 1200)
        #define SEEDY_WIDTH 2
        typedef unsigned long uint32_t;
        typedef unsigned short uint16_t;
        typedef unsigned char uint8_t;
#       define _OPT16
#       define _OPT32

/* NO LONG LONG, ASSUMING WIN32 TARGET */
#   elif (_MSC_VER < 1310)
        #include "stdintms.h"
        #define SEEDY_WIDTH 4
        typedef uint32_t seedy_t;
#       define _OPT16
#       define _OPT32

/* NO STDINT but LONG LONG is available */
#   elif (_MSC_VER < 1930) 
#       include "stdintms.h"
#       define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;
#       define _OPT16
#       define _OPT32
#       define _OPT64

/* STDINT is available... no shim needed. */
#   else
        #include <stdint.h>
        #define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;
#       define _OPT16
#       define _OPT32
#       define _OPT64
#   endif

#else /* ASSUMING MODERN COMPILER */

#   include <stdint.h>
#   define SEEDY_WIDTH 8
    typedef uint64_t seedy_t;
#   define _OPT16
#   define _OPT32
#   define _OPT64

#endif /* END STANDARD INT SUPPORT AND SHIMS.*/

/*
 *  SLEEP AND THREADS. TODO: FIND BETTER TIMER FOR WINDOWS
 */

#if defined(_WIN32)
#   include <windows.h>
#   include <wincrypt.h>
#   define wait_ms(ms) Sleep(ms)
#   define wait_us(ms) Sleep(ms)
#else
#   include <pthread.h>
#   include <unistd.h>
#   include <sys/random.h>
#   define _OPTARC4
#   define wait_ms(ms) usleep(ms * 1000)
#   define wait_us(us) usleep(us)
#endif

/*
 *   Standard C
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void stdinput(uint8_t* buffer, size_t bytes);
void* parseinputgen(int argc, char** argv);


/* OVERRIDE
#define _OPT64
#define _OPT32
#define _OPT16
*/

#if defined(_OPT64)
#   include "seedy64.h"
#endif
#if defined(_OPT32)      
#   include "seedy32.h"
#endif
#if defined(_OPT16) 
#   include "seedy16.h"
#endif

#endif // _SEEDY_H
