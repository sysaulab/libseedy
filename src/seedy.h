#ifndef _SEEDY_H
#define _SEEDY_H


/*****************************************************************************
 *                                                                           *
 *   stdint.h STANDARD INT SUPPORT AND SHIMS.                                *
 *                                                                           *
 *****************************************************************************/




/*
 *    BORLAND TURBO C DOS ...
 */

#if defined(__TURBOC__)

    typedef unsigned char  uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned long  uint32_t;
    typedef char           int8_t;
    typedef short          int16_t;
    typedef long           int32_t;
#   define _OPT16
#   define _OPT32

/*
 *    MICROSOFT COMPILERS
 */

#elif defined(_MSC_VER) /* MICROSOFT COMPILER */
#define MSVS_1 800
#define MSVS_2 900
#define MSVS_4 1000
#define MSVS_6 1200 /* wincrypt.h is not available */
#define MSVS_2002 1300 
#define MSVS_2003 1310 /* long long */
#define MSVS_2005 1400
#define MSVS_2008 1500
#define MSVS_2010 1600
#define MSVS_2012 1700
#define MSVS_2013 1800
#define MSVS_2015 1900 /* <stdint.h> */
#define MSVS_2017 1910
#define MSVS_2019 1920
#define MSVS_2022 1930

/* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING 16 bit TARGET */
#   if (_MSC_VER < MSVS_6)
        #define SEEDY_WIDTH 2
        #include "stdintms.h"
#       define _OPT16

/* NO LONG LONG */
#   elif (_MSC_VER < MSVS_2003) 
        #include "stdintms.h"
        #define SEEDY_WIDTH 4
        typedef uint32_t seedy_t;
#       define _OPT16
#       define _OPT32

/* NO STDINT but LONG LONG is available */
#   elif (_MSC_VER < MSVS_2015) 
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
#   if (_MSC_VER >= 1300) 
#   include <wincrypt.h>
#   define _OPTWINCRYPT
#   endif
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
