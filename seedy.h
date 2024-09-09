#ifndef _SEEDY_H
#define _SEEDY_H

#define _SEEDY_PASSIVE
/*
 *  INITIATING TYPES ACCORDING TO COMPILER TYPE
 */

#if defined(_MSC_VER) /* MICROSOFT COMPILER */

    /* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING WIN16 TARGET */
    #if (_MSC_VER < 1200)
        #define SEEDY_WIDTH 2
        typedef unsigned short seedy_t;
        typedef unsigned char uint8_t;
        #define _SEEDY_PASSIVE

    /* NO LONG LONG, ASSUMING WIN32 TARGET */
    #elif (_MSC_VER < 1310)

        #include "stdint.h"
        #define SEEDY_WIDTH 4
        typedef uint32_t seedy_t;

    /* NO STDINT but LONG LONG is available */
    #elif (_MSC_VER < 1930) 

        #include "stdint.h"
        #define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;

    /* STDINT is available... modern compiler */
    #else

        #include <stdint.h>
        #define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;

    #endif

#else /* C99 */

    #include <stdint.h>
    #define SEEDY_WIDTH 8
    typedef uint64_t seedy_t;

#endif



/*
 *  SLEEP AND THREADS
 */

#if defined(_WIN32)
    #include <windows.h>
    #define wait_ms(ms) Sleep(ms)
#else
    #include <pthread.h>
    #include <unistd.h>
    #define wait_ms(ms) usleep(ms*1000)
#endif



/*
 *  API
 */

void seedy(uint8_t* buffer, size_t bytes);



#endif
